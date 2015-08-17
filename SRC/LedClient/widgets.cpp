#include "widgets.h"

//////////////////////////////////////
///          LedWidget             ///
//////////////////////////////////////


LedWidget::LedWidget(QLabel *parent) : mBlinkState(false)
{
    mTimer = new QTimer(this);
    setFixedSize(20,20);

    connect(mTimer, SIGNAL(timeout()), this, SLOT(blink()));

    setColor(mLed.getLedColor());
    setRate(mLed.getLedRate());
}

bool LedWidget::setLedColor(const LedColor color)
{
    bool result = mLed.setLedColor(color);

    if(result){
        setColor(color);
    }

    return result;
}

bool LedWidget::setLedState(const LedState state)
{
    bool result = mLed.setLedState(state);

    if(result)
    {
        setState(state);

        if(state == LED_STATE_ON){
            setRate(mLed.getLedRate());
        }
    }

    return result;
}

bool LedWidget::setLedRate(const LedRate rate)
{
    bool result =  mLed.setLedRate(rate);

    if(result){
        setRate(rate);
    }

    return result;
}

LedColor LedWidget::getLedColor() const
{
    return mLed.getLedColor();
}

LedState LedWidget::getLedState() const
{
    return mLed.getLedState();
}

LedRate LedWidget::getLedRate() const
{
    return mLed.getLedRate();
}

void LedWidget::reset()
{
    mLed = Led();
    mTimer->stop();

    setColor(mLed.getLedColor());
    setRate(mLed.getLedRate());
}

void LedWidget::setColor(const LedColor color)
{
    if(mLed.getLedState() == LED_STATE_OFF)
    {
        setStyleSheet("background-color: grey; border: 2px solid green; border-radius: 4px;");
        return;
    };

    if(color == LED_COLOR_RED){
        setStyleSheet("background-color: red; border: 2px solid green; border-radius: 4px;");
    }
    else if(color == LED_COLOR_GREEN){
        setStyleSheet("background-color: green; border: 2px solid green; border-radius: 4px;");
    }
    else if(color == LED_COLOR_BLUE){
        setStyleSheet("background-color: blue; border: 2px solid green; border-radius: 4px;");
    }
}

void LedWidget::setState(const LedState state)
{
    if(state == LED_STATE_ON){
        setColor(mLed.getLedColor());
    }
    else if(state == LED_STATE_OFF){
        setColor(LED_COLOR_INVALID);
    }
}

void LedWidget::setRate(const LedRate rate)
{
    if(rate > 0)
    {
        quint16 timeout = 500/rate;
        mTimer->start(timeout);
    }
}

void LedWidget::blink()
{
    if(mLed.getLedState() == LED_STATE_OFF)
    {
        mTimer->stop();
        setColor(mLed.getLedColor());
        mBlinkState = false;
        return;
    }

    if(mBlinkState){
        setStyleSheet("background-color: grey; border: 2px solid green; border-radius: 4px;");
    }
    else{
        setColor(mLed.getLedColor());
    }

    mBlinkState = !mBlinkState;
}

//////////////////////////////////////
///      ConnectionWidget          ///
//////////////////////////////////////

ConnectionDialog::ConnectionDialog(QDialog *parent) : QDialog(parent)
{
    setModal(true);
    setWindowTitle("Connection Settings");

    QLabel* addrLabel = new QLabel("Address", this);
    QLabel* portLabel = new QLabel("Port", this);

    mAddr = new QLineEdit(this);
    mAddr->setText("127.0.0.1");

    QRegExp rx( "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}" );
    QRegExpValidator* regValidator = new QRegExpValidator( rx, this);
    mAddr->setValidator( regValidator );

    mPort = new QLineEdit(this);
    mPort->setText("8000");
    QIntValidator* inVal = new QIntValidator(this);
    mPort->setValidator(inVal);

    mConnectButton = new QPushButton ("Connect", this);
    mConnectButton->setFixedWidth(100);

    QHBoxLayout *settLay = new QHBoxLayout();
    settLay->addWidget(addrLabel);
    settLay->addWidget(mAddr);
    settLay->addWidget(portLabel);
    settLay->addWidget(mPort);

    QGroupBox* connBox = new QGroupBox("Connection Parameters", this);
    connBox->setLayout(settLay);

    QVBoxLayout* mainLay = new QVBoxLayout();
    setLayout(mainLay);

    mainLay->addWidget(connBox);
    mainLay->addWidget(mConnectButton);
    mainLay->setAlignment(mConnectButton, Qt::AlignCenter);

    connect(mConnectButton, SIGNAL(pressed()), this, SLOT(onConnectionButtonPressed()));
}

void ConnectionDialog::onConnectionButtonPressed()
{
    QHostAddress addr (mAddr->text());
    quint16 port = mPort->text().toUShort();

    emit connectToServer(addr, port);

    hide();
}

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

    mStateEditor->setFixedWidth(80);
    mColorEditor->setFixedWidth(80);
    mRateEditor->setFixedWidth(80);

    mStateSetButton = new QPushButton ("Set state", this);
    mStateGetButton = new QPushButton ("Get state", this);
    mColorSetButton = new QPushButton ("Set color", this);
    mColorGetButton = new QPushButton ("Get color", this);
    mRateSetButton = new QPushButton ("Set rate", this);
    mRateGetButton = new QPushButton ("Get rate", this);

    QHBoxLayout* stateLay = new QHBoxLayout;
    stateLay->addWidget(mStateEditor);
    stateLay->addWidget(mStateSetButton);
    stateLay->addWidget(mStateGetButton);

    QHBoxLayout* colorLay = new QHBoxLayout;
    colorLay->addWidget(mColorEditor);
    colorLay->addWidget(mColorSetButton);
    colorLay->addWidget(mColorGetButton);

    QHBoxLayout* rateLay = new QHBoxLayout;
    rateLay->addWidget(mRateEditor);
    rateLay->addWidget(mRateSetButton);
    rateLay->addWidget(mRateGetButton);


    QVBoxLayout* verticalLay = new QVBoxLayout;
    verticalLay->addLayout(stateLay);
    verticalLay->addLayout(colorLay);
    verticalLay->addLayout(rateLay);

    QGroupBox* box = new QGroupBox("Led Parameters", this);
    box->setLayout(verticalLay);

    QVBoxLayout* mainLay = new QVBoxLayout;
    mainLay->addWidget(box);
    setLayout(mainLay);

    connect (mStateSetButton, SIGNAL (clicked()), SLOT(setStateSlot()));
    connect (mColorSetButton, SIGNAL (clicked()), SLOT(setColorSlot()));
    connect (mRateSetButton, SIGNAL (clicked()), SLOT(setRateSlot()));

    connect (mStateGetButton, SIGNAL (clicked()), SIGNAL(getLedState()));
    connect (mColorGetButton, SIGNAL (clicked()), SIGNAL(getLedColor()));
    connect (mRateGetButton, SIGNAL (clicked()), SIGNAL(getLedRate()));
}

void LedSettingsWidget::setStateSlot()
{
    emit setLedState((LedState)mStateEditor->currentIndex());
}

void LedSettingsWidget::setColorSlot()
{
    emit setLedColor((LedColor)mColorEditor->currentIndex());
}

void LedSettingsWidget::setRateSlot()
{
    emit setLedRate((LedRate)mRateEditor->text().remove("Hz").toUShort());
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
    mRateEditor->setText(QString::number(rate) + " Hz");
}

bool LedSettingsWidget::setAvailable(bool isAvailable)
{
    mStateEditor->setDisabled(!isAvailable);
    mColorEditor->setDisabled(!isAvailable);
    mRateEditor->setDisabled(!isAvailable);
    mStateSetButton->setDisabled(!isAvailable);
    mStateGetButton->setDisabled(!isAvailable);
    mColorSetButton->setDisabled(!isAvailable);
    mColorGetButton->setDisabled(!isAvailable);
    mRateSetButton->setDisabled(!isAvailable);
    mRateGetButton->setDisabled(!isAvailable);
}
