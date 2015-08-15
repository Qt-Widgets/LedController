#ifndef LEDSERVER_H
#define LEDSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QByteArray>
#include <QTextEdit>

#include "ledlib.h"

using namespace LEDGLOBAL;

typedef QPair<QHostAddress, quint16> ClientId;

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

class Network : public QObject
{
    Q_OBJECT

    QTcpServer* mServer;
    quint16 mNextBlockSize;
    QHash<ClientId, QTcpSocket*> mConnectedClients;

public:
    Network( QObject* parent = 0);
    bool startServer(int port);

    void onNewCommands(const QString commands, ClientId id);
    void sendCommand(QString command, ClientId id);

public slots:
    virtual void onNewConnection();
    void onDisconnect();
    void onReadClient();

    void onSetLedStateResponse(bool ok, const LedState state, ClientId id);
    void onSetLedColorResponse(bool ok, const LedColor color, ClientId id);
    void onSetLedRateResponse(bool ok, const LedRate rate, ClientId id);

    void onGetLedStateResponse(const LedState state, ClientId id);
    void onGetLedColorResponse(const LedColor color, ClientId id);
    void onGetLedRateResponse(const LedRate rate, ClientId id);

signals:
    void setLedState(const LedState state, ClientId id);
    void setLedRate(const LedRate rate, ClientId id);
    void setLedColor(const LedColor color, ClientId id);

    void getLedState(ClientId id);
    void getLedColor(ClientId id);
    void getLedRate(ClientId id);

    void message(QString text);
};

//////////////////////////////////////
///           LedServer            ///
//////////////////////////////////////

class LedServer : public QMainWindow
{
    Q_OBJECT

    QTextEdit mTextEdit;
    Network mNet;

    Led mLed;

    void setState(LedState state);

public:
    explicit LedServer(QWidget *parent = 0);
    ~LedServer();

public slots:        
    void onSetLedState(const LedState state, ClientId id);
    void onSetLedColor(const LedColor color, ClientId id);
    void onSetLedRate(const LedRate rate, ClientId id);

    void onGetLedState(ClientId id);
    void onGetLedColor(ClientId id);
    void onGetLedRate(ClientId id);

    void onMessage(QString message);

signals:
    void setLedStateResponse(bool ok, const LedState state, ClientId id);
    void setLedColorResponse(bool ok, const LedColor color, ClientId id);
    void setLedRateResponse(bool ok, const LedRate rate, ClientId id);

    void getLedStateResponse(const LedState state, ClientId id);
    void getLedColorResponse(const LedColor color, ClientId id);
    void getLedRateResponse(const LedRate rate, ClientId id);
};

#endif // LEDSERVER_H
