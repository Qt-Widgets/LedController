#ifndef NET_H
#define NET_H

#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>

#include <memory>

#include "SRC/LedClient/ui_ledclient.h"
#include "ledlib.h"

using namespace LEDGLOBAL;

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

class Network : public QObject
{
    Q_OBJECT

    QTcpSocket* mSocket;
    quint16 mNextBlockSize;

    LedCommandType mWaitingCommand;

private:
    void handleCommand(const QString command);

public:
    Network(QObject* parent = 0);
    ~Network();

    void onNewCommands(const QString commands);
    void sendCommand(QString command);

public slots:
    void startClient(const QHostAddress serverAddr, const quint16 port);

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



#endif // LEDCLIENT_H

