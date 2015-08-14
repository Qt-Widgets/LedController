#ifndef LEDSERVER_H
#define LEDSERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QByteArray>
#include <QTextEdit>

#include "ledlib.h"

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

public slots:
    virtual void onNewConnection();
    void onDisconnect();
    void onReadClient();
    void onSendCommand(QString command, ClientId id);

signals:
    void newCommand(QString commandText, ClientId id);
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

    void setState(StateCommandHandler::State state);

public:
    explicit LedServer(QWidget *parent = 0);
    ~LedServer();

public slots:
    void onMessage(QString message);
    void onCommand(QString command, ClientId id);

signals:
    void sendCommand(QString command, ClientId id);
};

#endif // LEDSERVER_H
