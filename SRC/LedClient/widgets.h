#ifndef WIDGETS_H
#define WIDGETS_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QDockWidget>
#include <QRegExpValidator>
#include <QIntValidator>
#include <QMenu>
#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QHostAddress>

#include "ledlib.h"

using namespace LEDGLOBAL;

/**
*  Basic widgets used in the project
*/

//////////////////////////////////////
///          LedWidget             ///
//////////////////////////////////////

/**
*  Graphical led widget
*  Visualization of Led class
*/

class LedWidget : public QLabel
{
    Q_OBJECT

    Led mLed;
    QTimer* mTimer;

    bool mBlinkState;

    void setColor(const LedColor color);
    void setState(const LedState state);
    void setRate(const LedRate rate);

private slots:
    void blink();   /**< called on timer timeout, changes led state */

public:
    LedWidget(QLabel* parent = 0);

    bool setLedColor(const LedColor color);
    bool setLedState(const LedState state);
    bool setLedRate(const LedRate rate);

    LedColor getLedColor() const;
    LedState getLedState() const;
    LedRate getLedRate() const;

    void reset();   /**<  resets led to defaults */
};

//////////////////////////////////////
///      ConnectionWidget          ///
//////////////////////////////////////

class ConnectionDialog : public QDialog
{
    Q_OBJECT

    QLineEdit* mAddr;
    QLineEdit* mPort;

    QPushButton* mConnectButton;

public:
    ConnectionDialog(QDialog* parent = 0);

private slots:
    void onConnectionButtonPressed();

signals:
     void connectToServer(const QHostAddress addr, const quint16 port);
};

//////////////////////////////////////
///      LedSettingsWidget         ///
//////////////////////////////////////

/**
*  Provides means to control led parametes
*  Allows to set and get parameters
*/


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

    bool setAvailable(bool isAvailable); /**< disables and enables the widget depending on connection status*/

private slots:
    void setStateSlot();
    void setColorSlot();
    void setRateSlot();

signals:
    void setLedState(const LedState state);
    void setLedColor(const LedColor color);
    void setLedRate(const LedRate rate);
    void getLedState();
    void getLedColor();
    void getLedRate();
};

#endif // LEDCLIENT_H

