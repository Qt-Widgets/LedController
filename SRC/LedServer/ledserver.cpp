#include "ledserver.h"
#include "SRC/LedServer/ui_ledserver.h"

//////////////////////////////////////
///           LedServer            ///
//////////////////////////////////////

LedServer::LedServer(QWidget *parent) :QMainWindow(parent)
{
    connect(&mNet, SIGNAL(message(QString)), this, SLOT(onMessage(QString)));

    mNet.startServer(8015);
    setCentralWidget(&mTextEdit);
}

LedServer::~LedServer()
{

}

void LedServer::onMessage(QString message)
{
    mTextEdit.append(message);
}

void LedServer::onCommand(QString command, ClientId id)
{

}

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

Network::Network(QObject *parent) : QObject(parent), mNextBlockSize(0)
{

}

bool Network::startServer(int port)
{
    mServer = new QTcpServer(this);
    if(!mServer->listen(QHostAddress::Any, port))
    {
        emit message("Server : start failed");
        delete mServer;
        return false;
    }

    connect(mServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    emit message(QString("Server : listening to port %1").arg(port));

    return true;
}

void Network::onNewConnection()
{
   QTcpSocket* newSocket = mServer->nextPendingConnection();
   connect (newSocket, SIGNAL (readyRead()), this, SLOT(onReadClient()));
   connect (newSocket, SIGNAL (disconnected()), SLOT(onDisconnect()));

   mConnectedClients.insert(ClientId(newSocket->peerAddress(), newSocket->peerPort()), newSocket);

   emit message("Server : new connection");

   onSendCommand("Ololo", ClientId(newSocket->peerAddress(), newSocket->peerPort()));
}

void Network::onDisconnect()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    mConnectedClients.remove(ClientId(clientSocket->peerAddress(), clientSocket->peerPort()));
    clientSocket->deleteLater();

    emit message("Server : client disconnected");
}

void Network::onReadClient()
{
   QTcpSocket* clientSocket = (QTcpSocket*)sender();
   QDataStream in(clientSocket);

   while(true)
   {
        if(!mNextBlockSize)
        {
            if(clientSocket->bytesAvailable() < sizeof(mNextBlockSize)){
                break;
            }

            in >> mNextBlockSize;
        }

        if(clientSocket->bytesAvailable() < mNextBlockSize){
            break;
        }

        QString commandText;
        in>>commandText;

        emit message("Server onreadCLient() :" + commandText);

        emit newCommand(commandText, ClientId(clientSocket->peerAddress(), clientSocket->peerPort()));
   }
}

void Network::onSendCommand(QString command, ClientId id)
{
    emit message("Server onSendCommand() : " + command);

    auto socket = mConnectedClients.find(id);
    if(socket != mConnectedClients.end())
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out<<quint16(0)<<command;
        out.device()->seek(0);
        out<<quint16(arrBlock.size() - sizeof(quint16));

        (*socket)->write(arrBlock);
    }
}
