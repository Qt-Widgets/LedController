#ifndef NET_H
#define NET_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QByteArray>
#include <QDataStream>

#include "ledlib.h"
#include "ThreadPool.h"

using namespace LEDGLOBAL;

typedef qintptr ClientId;

//////////////////////////////////////
///           Client               ///
//////////////////////////////////////

/**
*  Handle connection for a client
*  Receives messages and sends responses for a specific client
*/

class Client : public QObject
{
    Q_OBJECT

    QTcpSocket* mSocket;
    qintptr mSocketDescriptor;
    quint16 mNextBlockSize;

public:
    Client(qintptr id, QObject* parent = 0);
    ~Client();

    void onNewCommands(const QString commands, const ClientId id);
    void sendCommand(QString command);

private:
    void handleCommand(const QString command);

public slots:
    void startClient();
    void readyRead();
    void onDisconnect();

    void onSetLedStateResponse(bool ok, const LedState state, const ClientId id);
    void onSetLedColorResponse(bool ok, const LedColor color, const ClientId id);
    void onSetLedRateResponse(bool ok, const LedRate rate, const ClientId id);

    void onGetLedStateResponse(const LedState state, const ClientId id);
    void onGetLedColorResponse(const LedColor color, const ClientId id);
    void onGetLedRateResponse(const LedRate rate, const ClientId id);

signals:
    void error(qintptr id, QTcpSocket::SocketError error);
    void disconnected(ClientId id);

    void setLedState(const LedState state, const ClientId id);
    void setLedRate(const LedRate rate, const ClientId id);
    void setLedColor(const LedColor color, const ClientId id);

    void getLedState(const ClientId id);
    void getLedColor(const ClientId id);
    void getLedRate(const ClientId id);

    void message(QString text);
};

//////////////////////////////////////
///             Server             ///
//////////////////////////////////////

/**
*  Multithreaded server
*  Handles incoming connections and creates their handler in separate threads
*  Serves as a proxy between ledserver and clients
*/

class Server : public QTcpServer
{
    Q_OBJECT

    typedef std::shared_ptr<Client> ClientPtr;

    QHash<ClientId, ClientPtr> mClients;

public:
    Server( QTcpServer* parent = 0);
    ~Server();

public slots:
    void startServer(quint16 port);
    void onDisconnect(ClientId clientId);

protected:
    void incomingConnection(qintptr socketDescriptor);  /**< Creates a client class object  and puts it into separate thread*/

signals:
    void message(QString text);

    void setLedState(const LedState state, const ClientId id);
    void setLedRate(const LedRate rate, const ClientId id);
    void setLedColor(const LedColor color, const ClientId id);

    void getLedState(const ClientId id);
    void getLedColor(const ClientId id);
    void getLedRate(const ClientId id);

    void setLedStateResponse(bool ok, const LedState state, const ClientId id);
    void setLedColorResponse(bool ok, const LedColor color, const ClientId id);
    void setLedRateResponse(bool ok, const LedRate rate, const ClientId id);

    void getLedStateResponse(const LedState state, const ClientId id);
    void getLedColorResponse(const LedColor color, const ClientId id);
    void getLedRateResponse(const LedRate rate, const ClientId id);
};


#endif // LEDSERVER_H
