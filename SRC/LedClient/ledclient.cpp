#include "ledclient.h"

/////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

LedClient::LedClient(QWidget *parent) :  QMainWindow(parent)
{
    qRegisterMetaType<LedState>("LedState");
    qRegisterMetaType<LedColor>("LedColor");
    qRegisterMetaType<LedRate>("LedRate");

    mNet = std::make_unique<Network>();

    //connects
    connect(&mConnectionWidget, SIGNAL(connectToServer(QHostAddress,quint16)), mNet.get(), SLOT(startClient(QHostAddress,quint16)));

    connect(&mSettingsWidget, SIGNAL(setLedState(LedState)), mNet.get(), SLOT(onSetLedState(LedState)));
    connect(&mSettingsWidget, SIGNAL(setLedColor(LedColor)), mNet.get(), SLOT(onSetLedColor(LedColor)));
    connect(&mSettingsWidget, SIGNAL(setLedRate(LedRate)),   mNet.get(), SLOT(onSetLedRate(LedRate)));

    connect(&mSettingsWidget, SIGNAL(getLedState()), mNet.get(), SLOT(onGetLedState()));
    connect(&mSettingsWidget, SIGNAL(getLedColor()), mNet.get(), SLOT(onGetLedColor()));
    connect(&mSettingsWidget, SIGNAL(getLedRate()),  mNet.get(), SLOT(onGetLedRate()));

    connect(mNet.get(), SIGNAL(ledStateResponse(LedState)), this, SLOT(onLedStateResponse(LedState)));
    connect(mNet.get(), SIGNAL(ledColorResponse(LedColor)), this, SLOT(onLedColorResponse(LedColor)));
    connect(mNet.get(), SIGNAL(ledRateResponse(LedRate)),   this, SLOT(onLedRateResponse(LedRate)));

    connect(mNet.get(), SIGNAL(connectionStatus(bool, QString)), this, SLOT(onConnectionStatus(bool,QString)));
    connect(mNet.get(), SIGNAL(message(QString)), this, SLOT(onMessage(QString)));

    mNetThread = new QThread;
    mNet->moveToThread(mNetThread);
    mNetThread->start();

    //actions
    QAction *showConnectionWidget = new QAction("&Connect to server", this);
    connect(showConnectionWidget, SIGNAL(triggered()), &mConnectionWidget, SLOT(show()));

    QMenu *menu;
    menu = menuBar()->addMenu("&Menu");
    menu->addAction(showConnectionWidget);

    //widgets
    setCentralWidget(&mSettingsWidget);
    QDockWidget *dock = new QDockWidget("Log", this);
    dock->setWidget(&textEdit);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    setFixedWidth(mSettingsWidget.width());
    mSettingsWidget.setAvailable(false);

    statusBar()->addWidget(&mLed);

    onMessage("Program started");
}

void LedClient::onLedStateResponse(const LedState state)
{
   mLed.setLedState(state);
   mSettingsWidget.setState(state);

   onMessage(QString("State response : %1")
             .arg(LED_COMMAND_HANDLERS::stateCommandHandler.stateToStr(state)));
}

void LedClient::onLedColorResponse(const LedColor color)
{
    mLed.setLedColor(color);
    mSettingsWidget.setColor(color);

    onMessage(QString("Color response : %1")
              .arg(LED_COMMAND_HANDLERS::colorCommandHandler.colorToStr(color)));
}

void LedClient::onLedRateResponse(const LedRate rate)
{
    mLed.setLedRate(rate);
    mSettingsWidget.setRate(rate);

    onMessage(QString("Rate response : %1 Hz").arg(rate));
}

void LedClient::onMessage(QString message)
{
    textEdit.append(">" + message.remove("\n"));
}

LedClient::~LedClient()
{
    mNetThread->quit();
    if (!mNetThread->wait(5000)) /**< if the thread has not exited properly */
    {
        mNetThread->terminate();
        mNetThread->wait();
    }

    delete mNetThread;
}

void LedClient::onConnectionStatus(bool isConnected, QString errorText)
{
    if(!isConnected){
        mLed.reset();
    }

    if(!errorText.isEmpty()){
        onMessage(errorText);
    }

    mSettingsWidget.setAvailable(isConnected);
}
