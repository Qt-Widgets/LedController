#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <QThread>
#include <QMutex>
#include <QHash>
#include <QList>
#include <QDebug>

#include <memory>
#include <mutex>

class Thread;

//////////////////////////////////////////////////////////////////////////////
///////////////              ThreadPool                 //////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
*  Provides a simple singleton-type thread pool to avoid an unnecessary creation/stopping of threads
*  and centralize their creation and destruction (the pool automatically stops all created threads on it's destruction)
*/

class ThreadPool
{
    typedef std::shared_ptr<Thread> ThreadPtr;

    QMutex mMutex;
    quint16 mThreadCount;                                                   /**< Total number of created threads, used to give unique ids to new threads */
    static const int mWaitForClose = 5000;                                  /**< Thread quit timeout, on exceeding of that timeout a thread will be terminated */
    static const int mMaxUnusedThreads = 5;                                 /**< On exceeding of this number all threads returned to the pool will be stopped and deleted */

    QHash<quint16, ThreadPtr> mActiveThreads;
    QList<ThreadPtr> mUnusedThreads;

    ThreadPool();
    ThreadPool(const ThreadPool&);
    ThreadPool& operator=(ThreadPool&);

    void stopThread(ThreadPtr thread);                                      /**< Stops or terminates a thread */

public:
    static ThreadPool& getInstance();

    static Thread* getThread(QString threadName = QString("PoolThread"));
    static void freeThread(quint16 threadId);                               /**< Either returns the thread to the pool or stops it (depending on the number of unused threads) */

    ~ThreadPool();
};

//////////////////////////////////////////////////////////////////////////////
///////////////                 Thread                  //////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
*  A small thread wrapper just to provide the ability to distinguish threads
*/

class Thread : public QThread
{
    quint16 mThreadId;

public:
    Thread(quint32 id, QObject* parent = 0);
    quint16 getId() const;
};


#endif // LEDSERVER_H
