#include "ledserver.h"
#include "SRC/LedServer/ui_ledserver.h"

//////////////////////////////////////
///     ServerSettingsDialog       ///
//////////////////////////////////////
///
ServerSettingsDialog::ServerSettingsDialog(QDialog *parent) : QDialog(parent)
{
    setModal(true);
    setWindowTitle("Connection Settings");

    QLabel* portLabel = new QLabel("Port", this);

    mPort = new QLineEdit(this);
    mPort->setText("8000");
    QIntValidator* inVal = new QIntValidator(this);
    mPort->setValidator(inVal);

    mStartButton = new QPushButton ("Start server", this);
    mStartButton->setFixedWidth(150);

    QHBoxLayout *settLay = new QHBoxLayout();
    settLay->addWidget(portLabel);
    settLay->addWidget(mPort);

    QGroupBox* connBox = new QGroupBox("Server Parameters", this);
    connBox->setLayout(settLay);

    QVBoxLayout* mainLay = new QVBoxLayout();
    setLayout(mainLay);

    mainLay->addWidget(connBox);
    mainLay->addWidget(mStartButton);
    mainLay->setAlignment(mStartButton, Qt::AlignCenter);

    connect(mStartButton, SIGNAL(pressed()), this, SLOT(onStartButtonPressed()));

    setFixedSize(200, 100);
}

void ServerSettingsDialog::onStartButtonPressed()
{
    quint16 port = mPort->text().toUShort();
    emit startServer(port);
    hide();
}


//////////////////////////////////////
///           LedServer            ///
//////////////////////////////////////

LedServer::LedServer(QWidget *parent) :QMainWindow(parent)
{
    qRegisterMetaType<LedState>("LedState");
    qRegisterMetaType<LedColor>("LedColor");
    qRegisterMetaType<LedRate>("LedRate");
    qRegisterMetaType<ClientId>("ClientId");

    setCentralWidget(&mTextEdit);

    //connects
    connect(&mServerSettingsWidget,
            SIGNAL(startServer(quint16)),
            &mServer,
            SLOT(startServer(quint16)));

    //requests
    connect(&mServer,
            SIGNAL(message(QString)),
            this,
            SLOT(onMessage(QString)));

    connect(&mServer,
            SIGNAL(setLedState(LedState, ClientId)),
            this,
            SLOT(onSetLedState(LedState, ClientId)));

    connect(&mServer,
            SIGNAL(setLedColor(LedColor, ClientId)),
            this,
            SLOT(onSetLedColor(LedColor, ClientId)));

    connect(&mServer,
            SIGNAL(setLedRate(LedRate, ClientId)),
            this,
            SLOT(onSetLedRate(LedRate, ClientId)));

    connect(&mServer,
            SIGNAL(getLedState(ClientId)),
            this,
            SLOT(onGetLedState(ClientId)));

    connect(&mServer,
            SIGNAL(getLedColor(ClientId)),
            this,
            SLOT(onGetLedColor(ClientId)));

    connect(&mServer,
            SIGNAL(getLedRate(ClientId)),
            this,
            SLOT(onGetLedRate(ClientId)));

    //response
    connect(this,
            SIGNAL(setLedStateResponse(bool, LedState, ClientId)),
            &mServer,
            SIGNAL(setLedStateResponse(bool, LedState, ClientId)));

    connect(this,
            SIGNAL(setLedColorResponse(bool, LedColor, ClientId)),
            &mServer,
            SIGNAL(setLedColorResponse(bool, LedColor, ClientId)));

    connect(this,
            SIGNAL(setLedRateResponse(bool, LedRate, ClientId)),
            &mServer,
            SIGNAL(setLedRateResponse(bool, LedRate, ClientId)));

    connect(this,
            SIGNAL(getLedStateResponse(LedState, ClientId)),
            &mServer,
            SIGNAL(getLedStateResponse(LedState, ClientId)));

    connect(this,
            SIGNAL(getLedColorResponse(LedColor, ClientId)),
            &mServer,
            SIGNAL(getLedColorResponse(LedColor, ClientId)));

    connect(this,
            SIGNAL(getLedRateResponse(LedRate, ClientId)),
            &mServer,
            SIGNAL(getLedRateResponse(LedRate, ClientId)));

    //actions
    QAction *showServerSettingsWidget = new QAction("&Start server", this);
    connect(showServerSettingsWidget, SIGNAL(triggered()), &mServerSettingsWidget, SLOT(show()));

    QMenu *menu;
    menu = menuBar()->addMenu("&Menu");
    menu->addAction(showServerSettingsWidget);

    onMessage("Program started");
}

void LedServer::onSetLedState(const LedState state, ClientId id)
{
    QMutexLocker lock(&mMutex);

    bool ok = mLed.setLedState(state);

    onMessage(QString("State SET request | %1 | %2")
            .arg(LED_COMMAND_HANDLERS::stateCommandHandler.stateToStr(state))
            .arg(ok? "OK" : "FAIL"));

    emit setLedStateResponse(ok, state, id);
}

void LedServer::onSetLedColor(const LedColor color, ClientId id)
{
    QMutexLocker lock(&mMutex);

    bool ok = mLed.setLedColor(color);

    onMessage(QString("Color SET request | %1 | %2")
            .arg(LED_COMMAND_HANDLERS::colorCommandHandler.colorToStr(color))
             .arg(ok? "OK" : "FAIL"));

    emit setLedColorResponse(ok, color, id);
}

void LedServer::onSetLedRate(const LedRate rate, ClientId id)
{
    QMutexLocker lock(&mMutex);

    bool ok = mLed.setLedRate(rate);

    onMessage(QString("Rate SET request | %1 | %2 Hz")
            .arg(rate)
            .arg(ok? "OK" : "FAIL"));

    emit setLedRateResponse(ok, rate, id);
}

void LedServer::onGetLedState(ClientId id)
{
    onMessage(QString("State GET request"));
    emit getLedStateResponse(mLed.getLedState(), id);
}

void LedServer::onGetLedColor(ClientId id)
{
    onMessage(QString("Color GET request"));
    emit getLedColorResponse(mLed.getLedColor(), id);
}

void LedServer::onGetLedRate(ClientId id)
{
    onMessage(QString("Rate GET request"));
    emit getLedRateResponse(mLed.getLedRate(), id);
}

void LedServer::onMessage(QString message)
{
    mTextEdit.append(">" + message.remove("\n"));
}

