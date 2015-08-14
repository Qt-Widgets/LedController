#include "ledclient.h"

//////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

LedClient::LedClient(QWidget *parent) :  QMainWindow(parent)
{
    connect(&net, SIGNAL(message(QString)), this, SLOT(onMessage(QString)));

    net.startClient(QHostAddress("127.0.0.1"), 8015);
    setCentralWidget(&textEdit);

    setLedState(StateCommandHandler::STATE_ON);
}

LedClient::~LedClient()
{

}

void LedClient::setLedState(StateCommandHandler::State state)
{
    ColorCommandHandler* h = (ColorCommandHandler*)CommandPool::getCommandHandler(CommandPool::LED_COMMAND_COLOR);

    QString command = h->createRequest(ColorCommandHandler::STATE_ON);
    command.remove("\n");
    auto type = CommandPool::getCommandInfo(command);
    state = h->getStateFromCommand(command, type.second);

    command = h->createRequest(StateCommandHandler::STATE_OFF);
    command.remove("\n");
    type = CommandPool::getCommandInfo(command);
    state = h->getStateFromCommand(command, type.second);

    command = h->createResponse(AbstractCommandHandler::RESPONSE_OK, StateCommandHandler::STATE_ON);
    command.remove("\n");
    type = CommandPool::getCommandInfo(command);
    state = h->getStateFromCommand(command, type.second);

    command = h->createResponse(AbstractCommandHandler::RESPONSE_OK, StateCommandHandler::STATE_OFF);
    command.remove("\n");
    type = CommandPool::getCommandInfo(command);
    state = h->getStateFromCommand(command, type.second);

    command = h->createResponse(AbstractCommandHandler::RESPONSE_FAILED);
    command.remove("\n");
    type = CommandPool::getCommandInfo(command);
    state = h->getStateFromCommand(command, type.second);

    command = h->createResponse(AbstractCommandHandler::RESPONSE_OK, StateCommandHandler::STATE_OFF);

    int i=0;
}

void LedClient::onMessage(QString message)
{
    textEdit.append(message);
}

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

Network::Network(QObject *parent) : QObject(parent), mNextBLockSize (0)
{

}

void Network::startClient(QHostAddress serverAddr, int port)
{
    emit message(QString("Client  : on %1 : %2").arg(serverAddr.toString()).arg(port));

    mSocket = new QTcpSocket(this);
    mSocket->connectToHost(serverAddr, port);

    connect(mSocket, SIGNAL (connected()), this, SLOT(onConnect()));
    connect(mSocket, SIGNAL (readyRead()), this, SLOT(onReadServer()));
    connect(mSocket, SIGNAL (error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void Network::onConnect()
{
    emit message("Client : connected");
    emit connectionStatus(true);
}

void Network::onError(QAbstractSocket::SocketError error)
{
    QString errorType = (error == QAbstractSocket::HostNotFoundError? "Хост не найден" :
                         error == QAbstractSocket::RemoteHostClosedError? "Хост закрыл соединение" :
                         error == QAbstractSocket::RemoteHostClosedError? "Хост отклонил соединение" : "");

    QString errorText = QString("Error: %1").arg(errorType);

    emit message("Client onError() : " + errorText);

    emit connectionStatus(false, errorText);
}

void Network::onReadServer()
{
    QDataStream in(mSocket);

    while(true)
    {
        if(!mNextBLockSize)
        {
            if(mSocket->bytesAvailable() < sizeof(mNextBLockSize)){
                break;
            }

            in >> mNextBLockSize;
        }

        if(mSocket->bytesAvailable() < mNextBLockSize){
            break;
        }

        QString commandText;
        in>>commandText;

        emit message("Client onReadServer() : " + commandText);

        emit newCommand(commandText);
    }
}

void Network::onSendCommand(QString command)
{
     emit message("Client onSendCommand() : " + command);

    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out<<quint16(0)<<command;
    out.device()->seek(0);
    out<<quint16(arrBlock.size() - sizeof(mNextBLockSize));

    mSocket->write(arrBlock);
}
