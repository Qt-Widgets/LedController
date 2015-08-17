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

/**
*  Basic client class
*  Handles messages from net class and sends them to widgets and vise versa
*/

class LedClient : public QMainWindow
{
    Q_OBJECT

    QTextEdit mLogWidget;
    std::unique_ptr<Network> mNet;
    QThread* mNetThread;

    LedWidget mLed;                         /**< graphical led indicator */
    LedSettingsWidget mSettingsWidget;
    ConnectionDialog mConnectionWidget;

public:
    explicit LedClient(QWidget *parent = 0);
    ~LedClient();

public slots:  
    void onConnectionStatus(bool isConnected, QString errorText = QString()); /**< connection status update */

    void onLedStateResponse(const LedState state);  /**< state response from server */
    void onLedColorResponse(const LedColor color);  /**< color response from server */
    void onLedRateResponse(const LedRate rate);     /**< rate  response from server */

    void onMessage(QString message);
};

#endif // LEDCLIENT_H

