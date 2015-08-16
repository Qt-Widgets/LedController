#include "net.h"

//////////////////////////////////////
///             Client             ///
//////////////////////////////////////

Client::Client(qintptr id, QObject *parent) : QObject(parent), mNextBlockSize(0)
{
    mSocketDescriptor = id;
}

void Client::startClient()
{
    qDebug()<<"STARTED";
    mSocket = new QTcpSocket();

    if (!mSocket -> setSocketDescriptor(mSocketDescriptor))
    {
        emit error(mSocketDescriptor, mSocket->error());
        emit disconnected(mSocketDescriptor);
        return;
    }

    connect (mSocket, SIGNAL(readyRead()) ,     this, SLOT(readyRead()), Qt::DirectConnection);
    connect (mSocket, SIGNAL(disconnected()) ,  this, SLOT(onDisconnect()));
}

void Client::readyRead()
{
    QDataStream in(mSocket);

    while(true)
    {
         if(!mNextBlockSize)
         {
             if(mSocket->bytesAvailable() < sizeof(mNextBlockSize)){
                 break;
             }

             in >> mNextBlockSize;
         }

         if(mSocket->bytesAvailable() < mNextBlockSize){
             break;
         }

         QString commandText;
         in >> commandText;

         mNextBlockSize = 0;
         onNewCommands(commandText, mSocketDescriptor);
    }
}

void Client::onDisconnect()
{
    emit disconnected(mSocketDescriptor);
}

void Client::onNewCommands(const QString commands, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QStringList commandsList = LED_COMMAND_HANDLERS::getCommandList(commands);

    //emit message("Client -->  Server :  " + commands);
    QString command;

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
            else if(info.type == LED_COMMAND_STATE_GET)
            {
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

void Client::onSetLedStateResponse(bool ok, const LedState state, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = ok?
                LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(state) :
                LED_COMMAND_HANDLERS::stateCommandHandler.createResponse();

    handleCommand(command);
}

void Client::onSetLedColorResponse(bool ok, const LedColor color, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = ok?
                LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(color) :
                LED_COMMAND_HANDLERS::colorCommandHandler.createResponse();

    handleCommand(command);
}

void Client::onSetLedRateResponse(bool ok, const LedRate rate, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = ok?
                LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate) :
                LED_COMMAND_HANDLERS::rateCommandHandler.createResponse();

    handleCommand(command);
}

void Client::onGetLedStateResponse(const LedState state, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(state);
    handleCommand(command);
}

void Client::onGetLedColorResponse(const LedColor color, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(color);
    handleCommand(command);
}

void Client::onGetLedRateResponse(const LedRate rate, const ClientId id)
{
    if(id != mSocketDescriptor){
        return;
    }

    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate);
    handleCommand(command);
}

void Client::handleCommand(const QString command)
{
   if(!command.isEmpty()){
       sendCommand(command);
   }
   else{
       emit message("Invalid command");
   }
}

void Client::sendCommand(QString command)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out<<quint16(0)<<command;
    out.device()->seek(0);
    out<<quint16(arrBlock.size() - sizeof(quint16));

    mSocket->write(arrBlock);
}

Client::~Client()
{   
    mSocket->deleteLater();
}

//////////////////////////////////////
///             Server             ///
//////////////////////////////////////

Server::Server(QTcpServer *parent) : QTcpServer(parent)
{

}

void Server::startServer(quint16 port)
{       
    if(!listen(QHostAddress::Any, port)){
        emit message("Server : start failed");           
    }

    emit message(QString("Server : listening to port %1").arg(port));
}
void Server::incomingConnection(qintptr socketDescriptor)
{ 
   ClientPtr client = std::make_shared<Client>(socketDescriptor);
   Thread* t = ThreadPool::getThread();
   client->moveToThread(t);

   connect(t,            SIGNAL(started()),                        client.get(), SLOT(startClient()));

   connect(client.get(), SIGNAL(message(QString)),                 this,    SIGNAL(message(QString)));
   connect(client.get(), SIGNAL(disconnected(ClientId)),           this,    SLOT(onDisconnect(ClientId)));

   connect(client.get(), SIGNAL(setLedState(LedState, ClientId)),  this,    SIGNAL(setLedState(LedState, ClientId)));
   connect(client.get(), SIGNAL(setLedColor(LedColor, ClientId)),  this,    SIGNAL(setLedColor(LedColor, ClientId)));
   connect(client.get(), SIGNAL(setLedRate(LedRate, ClientId)),    this,    SIGNAL(setLedRate(LedRate, ClientId)));
   connect(client.get(), SIGNAL(getLedState(ClientId)),            this,    SIGNAL(getLedState(ClientId)));
   connect(client.get(), SIGNAL(getLedColor(ClientId)),            this,    SIGNAL(getLedColor(ClientId)));
   connect(client.get(), SIGNAL(getLedRate(ClientId)),             this,    SIGNAL(getLedRate(ClientId)));

   connect(this, SIGNAL(setLedStateResponse(bool, LedState, ClientId)), client.get(), SLOT(onSetLedStateResponse(bool, LedState, ClientId)));
   connect(this, SIGNAL(setLedColorResponse(bool, LedColor, ClientId)), client.get(), SLOT(onSetLedColorResponse(bool, LedColor, ClientId)));
   connect(this, SIGNAL(setLedRateResponse(bool, LedRate, ClientId)),   client.get(), SLOT(onSetLedRateResponse(bool, LedRate, ClientId)));
   connect(this, SIGNAL(getLedStateResponse(LedState, ClientId)),       client.get(), SLOT(onGetLedStateResponse(LedState, ClientId)));
   connect(this, SIGNAL(getLedColorResponse(LedColor, ClientId)),       client.get(), SLOT(onGetLedColorResponse(LedColor, ClientId)));
   connect(this, SIGNAL(getLedRateResponse(LedRate, ClientId)),         client.get(), SLOT(onGetLedRateResponse(LedRate, ClientId)));

   t->start();

   mClients.insert(socketDescriptor, client);
   emit message(QString("Server : new connection #%1").arg(socketDescriptor));
}

void Server::onDisconnect(ClientId clientId)
{
   auto client = mClients.find(clientId);
   if(client != mClients.end())
   {
       Thread* t = (Thread*)(*client)->thread();
       ThreadPool::freeThread(t->getId());
       mClients.remove(clientId);
   }

   emit message("Server : client disconnected");
}

Server::~Server()
{
    close();
    mClients.clear();
}


