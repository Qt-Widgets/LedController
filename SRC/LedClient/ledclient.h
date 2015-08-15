#ifndef LEDCLIENT_H
#define LEDCLIENT_H

#include <QMainWindow>
#include "SRC/LedClient/ui_ledclient.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QTextEdit>
#include <QHash>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QDockWidget>

#include "ledlib.h"

using namespace LEDGLOBAL;


//////////////////////////////////////
///      LedSettingsWidget         ///
//////////////////////////////////////

class LedSettingsWidget : public QWidget
{
    Q_OBJECT

    QComboBox* mStateEditor;
    QComboBox* mColorEditor;
    QLineEdit* mRateEditor;

    QPushButton* mStateSetButton;
    QPushButton* mStateGetButton;
    QPushButton* mColorSetButton;
    QPushButton* mColorGetButton;
    QPushButton* mRateSetButton;
    QPushButton* mRateGetButton;

public:
    LedSettingsWidget(QWidget* parent = 0);

    LedState getState() const;
    LedColor getColor() const;
    LedRate getRate() const;

    void setState(const LedState state);
    void setColor(const LedColor color);
    void setRate(const LedRate rate);

signals:
    void setStateRequest();
    void setColorRequest();
    void setRateRequest();
    void getStateRequest();
    void getColorRequest();
    void getRateRequest();
};

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

class Network : public QObject
{
    Q_OBJECT

    QTcpSocket* mSocket;
    quint16 mNextBlockSize;

    LedCommandType mWaitingCommand;

public:
    Network( QObject* parent = 0);
    void startClient(QHostAddress serverAddr, int port);

    void onNewCommands(const QString commands);
    void sendCommand(QString command);

public slots:
    void onConnect();
    void onError(QAbstractSocket::SocketError error);
    void onReadServer();

    void onSetLedState(const LedState state);
    void onSetLedColor(const LedColor color);
    void onSetLedRate(const LedRate rate);

    void onGetLedState();
    void onGetLedColor();
    void onGetLedRate();

signals:
    void connectionStatus(bool isConnected, QString errorText = QString());

    void ledStateResponse(const LedState state);
    void ledColorResponse(const LedColor state);
    void ledRateResponse(const LedRate state);

    void message(QString text);
};

//////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

class LedClient : public QMainWindow
{
    Q_OBJECT

    QTextEdit textEdit;
    Network mNet;

    Led mLed;
    LedSettingsWidget mSettingsWidget;

public:
    explicit LedClient(QWidget *parent = 0);
    ~LedClient();

public slots:
    void setLedStateRequest();
    void setLedColorRequest();
    void setLedRateRequest();

    void getLedStateRequest();
    void getLedColorRequest();
    void getLedRateRequest();

    void onLedStateResponse(const LedState state);
    void onLedColorResponse(const LedColor color);
    void onLedRateResponse(const LedRate rate);

    void onMessage(QString message);

signals:
    void setLedState(const LedState state);
    void setLedColor(const LedColor color);
    void setLedRate(const LedRate rate);

    void getLedState();
    void getLedColor();
    void getLedRate();
};

#endif // LEDCLIENT_H

