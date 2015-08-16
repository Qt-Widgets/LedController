#include "ThreadPool.h"



//////////////////////////////////////////////////////////////////////////////
///////////////	                ThreadPool              //////////////////////
//////////////////////////////////////////////////////////////////////////////

ThreadPool::ThreadPool() : mThreadCount(0)
{

}

Thread* ThreadPool::getThread(QString threadName)
{
    ThreadPool& p = getInstance();
    QMutexLocker locker(&p.mMutex);

    ThreadPtr thread;

    if (!p.mUnusedThreads.isEmpty()) /**< if there are some unused threads, we take one */
    {
        thread = *p.mUnusedThreads.begin();
        p.mUnusedThreads.erase(p.mUnusedThreads.begin());
    }
    else /**< we create and start new thread */
    {
        thread = std::make_shared<Thread>(p.mThreadCount);
        thread->start();
        p.mThreadCount++;
    }

    thread->setObjectName(threadName);
    p.mActiveThreads.insert(thread->getId(), thread); /**< the thread is not cosidered active */

    return thread.get();
}

void ThreadPool::freeThread(quint16 threadId)
{
    ThreadPool& p = getInstance();
    QMutexLocker locker(&p.mMutex);

    auto unusedThread = p.mActiveThreads.find(threadId);
    if (unusedThread != p.mActiveThreads.end())
    {
        if (p.mUnusedThreads.size() < mMaxUnusedThreads){ /**< if the current number of unused threads is less than their maximum number */
            p.mUnusedThreads.append(*unusedThread);
        }
        else{
            p.stopThread(*unusedThread);
        }

        p.mActiveThreads.erase(unusedThread);
    }
}

void ThreadPool::stopThread(ThreadPtr thread)
{
    thread->quit();
    if (!thread->wait(mWaitForClose)) /**< if the thread has not exited properly */
    {
        thread->terminate();
        thread->wait();
    }
}

ThreadPool &ThreadPool::getInstance()
{
    static ThreadPool instance;
    return instance;
}

ThreadPool::~ThreadPool()
{
    /**< stopping and deleting all created threads */

    qDebug()<<"~Pool";

    for (auto thread : mActiveThreads){
        stopThread(thread);
    }

    for (auto thread : mUnusedThreads){
        stopThread(thread);
    }

    mActiveThreads.clear();
    mUnusedThreads.clear();
}

//////////////////////////////////////////////////////////////////////////////
///////////////                     Thread              //////////////////////
//////////////////////////////////////////////////////////////////////////////

Thread::Thread(quint32 id, QObject* parent /*= 0*/) : mThreadId(id), QThread(parent)
{

}

quint16 Thread::getId() const
{
    return mThreadId;
}
