#ifndef LEDTESTS_H
#define LEDTESTS_H

#include <QtTest/QtTest>
#include "ledlib.h"

//////////////////////////////////////
///            LedTest             ///
//////////////////////////////////////

class LedTest : public QObject
{
    Q_OBJECT

public:
    LedTest( QObject* parent = 0);

private slots:
    void testState();
    void testColor();
    void testRate();
};

#endif // LEDTESTS_H
