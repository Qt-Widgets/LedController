#ifndef LEDSERVER_H
#define LEDSERVER_H

#include <QMainWindow>
#include <QByteArray>
#include <QTextEdit>
#include <QIntValidator>
#include <QMenu>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

#include "ledlib.h"
#include "net.h"

using namespace LEDGLOBAL;

Q_DECLARE_METATYPE(LedState);
Q_DECLARE_METATYPE(LedColor);
Q_DECLARE_METATYPE(LedRate);
Q_DECLARE_METATYPE(ClientId);

//////////////////////////////////////
///     ServerSettingsDialog       ///
//////////////////////////////////////

class ServerSettingsDialog : public QDialog
{
    Q_OBJECT

    QLineEdit* mPort;
    QPushButton* mStartButton;

public:
    ServerSettingsDialog(QDialog* parent = 0);

private slots:
    void onStartButtonPressed();

signals:
     void startServer(const quint16 port);
};

//////////////////////////////////////
///           LedServer            ///
//////////////////////////////////////

/**
*  Basic server class
*  Handles messages from net class and responds to them
*/


class LedServer : public QMainWindow
{
    Q_OBJECT

    QTextEdit mTextEdit;
    Server mServer;
    Led mLed;

    QMutex mMutex;

    ServerSettingsDialog mServerSettingsWidget;

public:
    explicit LedServer(QWidget *parent = 0);

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
