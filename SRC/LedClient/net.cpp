#include "net.h"

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

Network::Network(QObject *parent) :
    mNextBlockSize (0),
    mSocket(nullptr),
    mWaitingCommand (LED_COMMAND_INVALID),
    QObject(parent)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    qRegisterMetaType<QHostAddress>("QHostAddress");
}

void Network::onNewCommands(const QString commands)
{
    QStringList commandsList = LED_COMMAND_HANDLERS::getCommandList(commands);

    //emit message("Server -->  Client :  " + commands);

    for (auto command : commandsList)
    {
        auto info = LED_COMMAND_HANDLERS::getCommandInfo(command);

        if(info.type == LED_COMMAND_RESPONSE_OK)
        {
            if(info.param == LED_PARAM_STATE)
            {
              LedState state =  LED_COMMAND_HANDLERS::stateCommandHandler.getState(command);
              emit ledStateResponse(state);
            }
            else if(info.param == LED_PARAM_COLOR)
            {
                LedColor color =  LED_COMMAND_HANDLERS::colorCommandHandler.getColor(command);
                emit ledColorResponse(color);
            }
            else if(info.param == LED_PARAM_RATE)
            {
                LedRate rate =  LED_COMMAND_HANDLERS::rateCommandHandler.getRate(command);
                emit ledRateResponse(rate);
            }
            else{
                emit message("Last task succeeded");
            }
        }
        else if(info.type == LED_COMMAND_RESPONSE_FAILED)
        {
            emit message("Task failed");
        }
    }
}

void Network::onSetLedState(const LedState state)
{
    QString command = LED_COMMAND_HANDLERS::stateCommandHandler.createSetStateCommand(state);
    handleCommand(command);
}

void Network::onSetLedColor(const LedColor color)
{
    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(color);
    handleCommand(command);
}

void Network::onSetLedRate(const LedRate rate)
{
    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(rate);
    handleCommand(command);
}

void Network::onGetLedState()
{
    QString command = LED_COMMAND_HANDLERS::stateCommandHandler.createGetStateCommand();
    handleCommand(command);
}

void Network::onGetLedColor()
{
    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createGetColorCommand();
    handleCommand(command);
}

void Network::onGetLedRate()
{
    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createGetRateCommand();
    handleCommand(command);
}

void Network::startClient(const QHostAddress serverAddr, const quint16 port)
{
    emit message(QString("Connecting to %1 : %2").arg(serverAddr.toString()).arg(port));

    if(mSocket){
        delete mSocket;
    }

    mSocket = new QTcpSocket();
    mSocket->connectToHost(serverAddr, port);

    connect(mSocket, SIGNAL (connected()), this, SLOT(onConnect()));
    connect(mSocket, SIGNAL (readyRead()), this, SLOT(onReadServer()));
    connect(mSocket, SIGNAL (error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void Network::onConnect()
{
    emit message("Connected to server");
    emit connectionStatus(true);
}

void Network::onError(QAbstractSocket::SocketError error)
{
    QString errorType = (error == QAbstractSocket::HostNotFoundError? "Host not found" :
                         error == QAbstractSocket::ConnectionRefusedError? "Connection timed out" :
                         error == QAbstractSocket::RemoteHostClosedError? "Connection refused" : "Unknown error");

    QString errorText = QString("Error: %1").arg(errorType);

    mWaitingCommand = LED_COMMAND_INVALID;
    emit connectionStatus(false, errorText);
}

void Network::onReadServer()
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

        onNewCommands(commandText);
    }
}

void Network::sendCommand(QString command)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);

    out<<quint16(0)<<command;
    out.device()->seek(0);
    out<<quint16(arrBlock.size() - sizeof(mNextBlockSize));

    mSocket->write(arrBlock);

    //emit message("Client --> Server : " + command);
}


void Network::handleCommand(const QString command)
{
    if(mSocket == nullptr){
        emit message("Connection to a server required");
        return;
    }

    if(command.isEmpty()){
        emit message ("Wrong command");
    }
    else{
        sendCommand(command);
    }
}

Network::~Network()
{
    if(mSocket){
        mSocket->deleteLater();
    }
}
