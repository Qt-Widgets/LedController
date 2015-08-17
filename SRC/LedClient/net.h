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

/**
*  Basic network class
*  Sends messages to server and reads them from it
*/

class Network : public QObject
{
    Q_OBJECT

    QTcpSocket* mSocket;
    quint16 mNextBlockSize;

private:
    void handleCommand(const QString command);

public:
    Network(QObject* parent = 0);
    ~Network();

    void onNewCommands(const QString commands); /**< determines the type of a command and sends it to ledClient */
    void sendCommand(QString command);          /**< send command to server */

public slots:
    void startClient(const QHostAddress serverAddr, const quint16 port);

    void onConnect();
    void onError(QAbstractSocket::SocketError error);
    void onReadServer();

    //commands from client
    void onSetLedState(const LedState state);
    void onSetLedColor(const LedColor color);
    void onSetLedRate(const LedRate rate);

    void onGetLedState();
    void onGetLedColor();
    void onGetLedRate();

signals:
    void connectionStatus(bool isConnected, QString errorText = QString());  /**< connection status update */

    //responses from server
    void ledStateResponse(const LedState state);
    void ledColorResponse(const LedColor state);
    void ledRateResponse(const LedRate state);

    void message(QString text);
};



#endif // LEDCLIENT_H

