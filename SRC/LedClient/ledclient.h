#ifndef LEDCLIENT_H
#define LEDCLIENT_H

#include <QMainWindow>
#include <QTextEdit>
#include <QThread>

#include <memory>

#include "SRC/LedClient/ui_ledclient.h"
#include "ledlib.h"
#include "widgets.h"
#include "net.h"

using namespace LEDGLOBAL;

Q_DECLARE_METATYPE(LedState);
Q_DECLARE_METATYPE(LedColor);
Q_DECLARE_METATYPE(LedRate);

//////////////////////////////////////
///             LedClient          ///
//////////////////////////////////////

class LedClient : public QMainWindow
{
    Q_OBJECT

    QTextEdit textEdit;
    std::unique_ptr<Network> mNet;
    QThread* mNetThread;

    LedWidget mLed;
    LedSettingsWidget mSettingsWidget;
    ConnectionDialog mConnectionWidget;

public:
    explicit LedClient(QWidget *parent = 0);
    ~LedClient();

public slots:  
    void onConnectionStatus(bool isConnected, QString errorText = QString());

    void onLedStateResponse(const LedState state);
    void onLedColorResponse(const LedColor color);
    void onLedRateResponse(const LedRate rate);

    void onMessage(QString message);
};

#endif // LEDCLIENT_H

