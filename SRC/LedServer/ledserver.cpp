#include "ledserver.h"
#include "SRC/LedServer/ui_ledserver.h"

//////////////////////////////////////
///           LedServer            ///
//////////////////////////////////////

LedServer::LedServer(QWidget *parent) :QMainWindow(parent)
{
    connect(&mNet, SIGNAL(message(QString)), this, SLOT(onMessage(QString)));
    connect(&mNet, SIGNAL(setLedState(LedState, ClientId)), this, SLOT(onSetLedState(LedState, ClientId)));
    connect(&mNet, SIGNAL(setLedColor(LedColor, ClientId)), this, SLOT(onSetLedColor(LedColor, ClientId)));
    connect(&mNet, SIGNAL(setLedRate(LedRate, ClientId)), this, SLOT(onSetLedRate(LedRate, ClientId)));
    connect(&mNet, SIGNAL(getLedState(ClientId)), this, SLOT(onGetLedState(ClientId)));
    connect(&mNet, SIGNAL(getLedColor(ClientId)), this, SLOT(onGetLedColor(ClientId)));
    connect(&mNet, SIGNAL(getLedRate(ClientId)), this, SLOT(onGetLedRate(ClientId)));

    connect(this, SIGNAL(setLedStateResponse(bool, LedState, ClientId)), &mNet, SLOT(onSetLedStateResponse(bool, LedState, ClientId)));
    connect(this, SIGNAL(setLedColorResponse(bool, LedColor, ClientId)), &mNet, SLOT(onSetLedColorResponse(bool, LedColor, ClientId)));
    connect(this, SIGNAL(setLedRateResponse(bool, LedRate, ClientId)), &mNet, SLOT(onSetLedRateResponse(bool, LedRate, ClientId)));
    connect(this, SIGNAL(getLedStateResponse(LedState, ClientId)), &mNet, SLOT(onGetLedStateResponse(LedState, ClientId)));
    connect(this, SIGNAL(getLedColorResponse(LedColor, ClientId)), &mNet, SLOT(onGetLedColorResponse(LedColor, ClientId)));
    connect(this, SIGNAL(getLedRateResponse(LedRate, ClientId)), &mNet, SLOT(onGetLedRateResponse(LedRate, ClientId)));

    mNet.startServer(8015);
    setCentralWidget(&mTextEdit);
}

void LedServer::onSetLedState(const LedState state, ClientId id)
{
    bool ok = mLed.setLedState(state);
    emit setLedStateResponse(ok, state, id);
}

void LedServer::onSetLedColor(const LedColor color, ClientId id)
{
    bool ok = mLed.setLedColor(color);
    emit setLedColorResponse(ok, color, id);
}

void LedServer::onSetLedRate(const LedRate rate, ClientId id)
{
    bool ok = mLed.setLedRate(rate);
    emit setLedRateResponse(ok, rate, id);
}

void LedServer::onGetLedState(ClientId id)
{
    emit getLedStateResponse(mLed.getLedState(), id);
}

void LedServer::onGetLedColor(ClientId id)
{
    emit getLedColorResponse(mLed.getLedColor(), id);
}

void LedServer::onGetLedRate(ClientId id)
{
    emit getLedRateResponse(mLed.getLedRate(), id);
}

void LedServer::onMessage(QString message)
{
    mTextEdit.append(message.remove("\n"));
}

LedServer::~LedServer()
{

}

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

Network::Network(QObject *parent) : QObject(parent), mNextBlockSize(0)
{

}

void Network::onNewCommands(const QString commands, ClientId id)
{
    QStringList commandsList = LED_COMMAND_HANDLERS::getCommandList(commands);

    emit message("Client -->  Server :  " + commands);

    for (auto command : commandsList)
    {
        auto info = LED_COMMAND_HANDLERS::getCommandInfo(command);

        if(info.param == LED_PARAM_STATE)
        {
            if(info.type == LED_COMMAND_STATE_SET)
            {
                LedState state = LED_COMMAND_HANDLERS::stateCommandHandler.getState(command);
                emit setLedState(state, id);
            }
            else if(info.type == LED_COMMAND_STATE_GET){
                emit getLedState(id);
            }
        }
        else if(info.param == LED_PARAM_COLOR)
        {
            if(info.type == LED_COMMAND_COLOR_SET)
            {
                LedColor color = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(command);
                emit setLedColor(color, id);
            }
            else if(info.type == LED_COMMAND_COLOR_GET){
                emit getLedColor(id);
            }
        }
        else if(info.param == LED_PARAM_RATE)
        {
            if(info.type == LED_COMMAND_RATE_SET)
            {
                LedRate rate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(command);
                emit setLedRate(rate, id);
            }
            else if(info.type == LED_COMMAND_RATE_GET){
                emit getLedRate(id);
            }
        }
        else{
            emit message("Invalid incoming command");
        }
    }
}

void Network::onSetLedStateResponse(bool ok, const LedState state, ClientId id)
{
    QString command = ok?
                LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(state) :
                LED_COMMAND_HANDLERS::stateCommandHandler.createResponse();

    sendCommand(command, id);
}

void Network::onSetLedColorResponse(bool ok, const LedColor color, ClientId id)
{
    QString command = ok?
                LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(color) :
                LED_COMMAND_HANDLERS::colorCommandHandler.createResponse();

    sendCommand(command, id);
}

void Network::onSetLedRateResponse(bool ok, const LedRate rate, ClientId id)
{
    QString command = ok?
                LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate) :
                LED_COMMAND_HANDLERS::rateCommandHandler.createResponse();

    sendCommand(command, id);
}

void Network::onGetLedStateResponse(const LedState state, ClientId id)
{
     QString command = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(state);
     sendCommand(command, id);
}

void Network::onGetLedColorResponse(const LedColor color, ClientId id)
{
    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(color);
    sendCommand(command, id);
}

void Network::onGetLedRateResponse(const LedRate rate, ClientId id)
{
    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate);
    sendCommand(command, id);
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
        in >> commandText;

        mNextBlockSize = 0;

        ClientId id(clientSocket->peerAddress(), clientSocket->peerPort());
        onNewCommands(commandText, id);
   }
}

void Network::sendCommand(QString command, ClientId id)
{   
    auto socket = mConnectedClients.find(id);
    if(socket != mConnectedClients.end())
    {
        QByteArray arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);

        out<<quint16(0)<<command;
        out.device()->seek(0);
        out<<quint16(arrBlock.size() - sizeof(quint16));

        (*socket)->write(arrBlock);

        emit message("Server -->  Client:  " + command);
    }
}
