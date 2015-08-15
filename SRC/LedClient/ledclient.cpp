#include "ledclient.h"


//////////////////////////////////////
///      LedSettingsWidget         ///
//////////////////////////////////////

LedSettingsWidget::LedSettingsWidget(QWidget *parent)
{
    mStateEditor = new QComboBox(this);
    for(int state = 0; state < LED_STATE_INVALID; ++ state){
        mStateEditor->addItem(LED_COMMAND_HANDLERS::stateCommandHandler.stateToStr((LedState)state));
    }

    mColorEditor = new QComboBox(this);
    for(int color = 0; color < LED_COLOR_INVALID; ++ color){
        mColorEditor->addItem(LED_COMMAND_HANDLERS::colorCommandHandler.colorToStr((LedColor)color));
    }

    mRateEditor = new QLineEdit(this);
    mRateEditor->setText("0");

    mStateSetButton = new QPushButton ("Set state", this);
    mStateGetButton = new QPushButton ("Get state", this);
    mColorSetButton = new QPushButton ("Set color", this);
    mColorGetButton = new QPushButton ("Get color", this);
    mRateSetButton = new QPushButton ("Set rate", this);
    mRateGetButton = new QPushButton ("Get rate", this);

   QGridLayout *layout = new QGridLayout();
   layout->addWidget(mStateEditor,0,2);
   layout->addWidget(mStateSetButton,0,3);
   layout->addWidget(mStateGetButton,0,4);
   layout->addWidget(mColorEditor,1,2);
   layout->addWidget(mColorSetButton,1,3);
   layout->addWidget(mColorGetButton,1,4);
   layout->addWidget(mRateEditor,2,2);
   layout->addWidget(mRateSetButton,2,3);
   layout->addWidget(mRateGetButton,2,4);

   QGroupBox* box = new QGroupBox("Led Parameters", this);
   box->setLayout(layout);

   setFixedSize(310,110);

   connect (mStateSetButton, SIGNAL (clicked()), SIGNAL(setStateRequest()));
   connect (mColorSetButton, SIGNAL (clicked()), SIGNAL(setColorRequest()));
   connect (mRateSetButton, SIGNAL (clicked()), SIGNAL(setRateRequest()));

   connect (mStateGetButton, SIGNAL (clicked()), SIGNAL(getStateRequest()));
   connect (mColorGetButton, SIGNAL (clicked()), SIGNAL(getColorRequest()));
   connect (mRateGetButton, SIGNAL (clicked()), SIGNAL(getRateRequest()));
}

LedState LedSettingsWidget::getState() const
{
    return (LedState)mStateEditor->currentIndex();
}

LedColor LedSettingsWidget::getColor() const
{
    return (LedColor)mColorEditor->currentIndex();
}

LedRate LedSettingsWidget::getRate() const
{
    return mRateEditor->text().toUShort();
}

void LedSettingsWidget::setState(const LedState state)
{
    mStateEditor->setCurrentIndex(state);
}

void LedSettingsWidget::setColor(const LedColor color)
{
    mColorEditor->setCurrentIndex(color);
}

void LedSettingsWidget::setRate(const LedRate rate)
{
    mRateEditor->setText(QString::number(rate));
}

//////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

LedClient::LedClient(QWidget *parent) :  QMainWindow(parent)
{
    connect(&mNet, SIGNAL(message(QString)), this, SLOT(onMessage(QString)));

    connect(this, SIGNAL(setLedState(LedState)), &mNet, SLOT(onSetLedState(LedState)));
    connect(this, SIGNAL(setLedColor(LedColor)), &mNet, SLOT(onSetLedColor(LedColor)));
    connect(this, SIGNAL(setLedRate(LedRate)), &mNet, SLOT(onSetLedRate(LedRate)));

    connect(this, SIGNAL(getLedState()), &mNet, SLOT(onGetLedState()));
    connect(this, SIGNAL(getLedColor()), &mNet, SLOT(onGetLedColor()));
    connect(this, SIGNAL(getLedRate()), &mNet, SLOT(onGetLedRate()));

    connect(&mNet, SIGNAL(ledStateResponse(LedState)), this, SLOT(onLedStateResponse(LedState)));
    connect(&mNet, SIGNAL(ledColorResponse(LedColor)), this, SLOT(onLedColorResponse(LedColor)));
    connect(&mNet, SIGNAL(ledRateResponse(LedRate)), this, SLOT(onLedRateResponse(LedRate)));

    connect(&mSettingsWidget, SIGNAL(setStateRequest()), this, SLOT(setLedStateRequest()));
    connect(&mSettingsWidget, SIGNAL(setColorRequest()), this, SLOT(setLedColorRequest()));
    connect(&mSettingsWidget, SIGNAL(setRateRequest()), this, SLOT(setLedRateRequest()));

    connect(&mSettingsWidget, SIGNAL(getStateRequest()), this, SLOT(getLedStateRequest()));
    connect(&mSettingsWidget, SIGNAL(getColorRequest()), this, SLOT(getLedColorRequest()));
    connect(&mSettingsWidget, SIGNAL(getRateRequest()), this, SLOT(getLedRateRequest()));

    setCentralWidget(&mSettingsWidget);
    QDockWidget *dock = new QDockWidget("Log", this);
    dock->setWidget(&textEdit);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    mNet.startClient(QHostAddress("127.0.0.1"), 8015);

    setFixedWidth(mSettingsWidget.width());
}

LedClient::~LedClient()
{

}

void LedClient::setLedStateRequest()
{
    LedState state = mSettingsWidget.getState();
    emit setLedState(state);
}

void LedClient::setLedColorRequest()
{
    LedColor color = mSettingsWidget.getColor();
    emit setLedColor(color);
}

void LedClient::setLedRateRequest()
{
    LedRate rate = mSettingsWidget.getRate();
    emit setLedRate(rate);
}

void LedClient::getLedStateRequest()
{
    emit getLedState();
}

void LedClient::getLedColorRequest()
{
     emit getLedColor();
}

void LedClient::getLedRateRequest()
{
     emit getLedRate();
}

void LedClient::onLedStateResponse(const LedState state)
{
   mLed.setLedState(state);
   mSettingsWidget.setState(state);
   onMessage("State response " + QString::number(state));
}

void LedClient::onLedColorResponse(const LedColor color)
{
    mLed.setLedColor(color);
    mSettingsWidget.setColor(color);
    onMessage("Color response " + QString::number(color));
}

void LedClient::onLedRateResponse(const LedRate rate)
{
    mLed.setLedRate(rate);
    mSettingsWidget.setRate(rate);
    onMessage("Rate response " + QString::number(rate));
}

void LedClient::onMessage(QString message)
{
    textEdit.append(message.remove("\n"));
}

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

Network::Network(QObject *parent) : QObject(parent), mNextBlockSize (0), mWaitingCommand (LED_COMMAND_INVALID)
{

}

void Network::onNewCommands(const QString commands)
{
    QStringList commandsList = LED_COMMAND_HANDLERS::getCommandList(commands);

    emit message("Server -->  Client :  " + commands);

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
    sendCommand(command);
}

void Network::onSetLedColor(const LedColor color)
{
    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(color);
    sendCommand(command);
}

void Network::onSetLedRate(const LedRate rate)
{
    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(rate);
    sendCommand(command);
}

void Network::onGetLedState()
{
    QString command = LED_COMMAND_HANDLERS::stateCommandHandler.createGetStateCommand();
    sendCommand(command);
}

void Network::onGetLedColor()
{
    QString command = LED_COMMAND_HANDLERS::colorCommandHandler.createGetColorCommand();
    sendCommand(command);
}

void Network::onGetLedRate()
{
    QString command = LED_COMMAND_HANDLERS::rateCommandHandler.createGetRateCommand();
    sendCommand(command);
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

    emit message("Client --> Server : " + command);
}



