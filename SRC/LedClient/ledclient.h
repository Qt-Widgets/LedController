#ifndef LEDCLIENT_H
#define LEDCLIENT_H

#include <QMainWindow>
#include "SRC/LedClient/ui_ledclient.h"

#include <QTcpSocket>
#include <QByteArray>
#include <QHostAddress>
#include <QTextEdit>

#include "ledlib.h"

//////////////////////////////////////
///             Network            ///
//////////////////////////////////////

class Network : public QObject
{
    Q_OBJECT

    QTcpSocket* mSocket;
    quint16 mNextBLockSize;

public:
    Network( QObject* parent = 0);
    void startClient(QHostAddress serverAddr, int port);

public slots:
    void onConnect();
    void onError(QAbstractSocket::SocketError error);
    void onReadServer();
    void onSendCommand(QString command);

signals:
    void connectionStatus(bool isConnected, QString errorText = QString());
    void newCommand(QString commandText);
    void message(QString text);
};

//////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

class LedClient : public QMainWindow
{
    Q_OBJECT

    QTextEdit textEdit;
    Network net;

    Led mLed;

public:
    explicit LedClient(QWidget *parent = 0);
    ~LedClient();

    void setLedState(StateCommandHandler::State state);

public slots:
    void onMessage(QString message);

signals:
    void sendCommand(QString command);
};

#endif // LEDCLIENT_H
