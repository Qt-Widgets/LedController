#include "ledtest.h"

using namespace LEDGLOBAL;

LedTest::LedTest(QObject *parent) : QObject(parent)
{

}

void LedTest::testState()
{
    //set-request, ok-response, valid states
    for(int state = 0; state < LED_STATE_INVALID;  ++state)
    {
        LedState ledState = (LedState)state;
        QString request = LED_COMMAND_HANDLERS::stateCommandHandler.createSetStateCommand(ledState);
        request.remove("\n");
        LedCommandType type = LED_COMMAND_HANDLERS::getCommandType(request);
        ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
        QVERIFY(type == LED_COMMAND_STATE_SET && ledState == state);

        request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(ledState);
        request.remove("\n");
        type = LED_COMMAND_HANDLERS::getCommandType(request);
        ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
        QVERIFY(type == LED_COMMAND_RESPONSE_OK && ledState == state);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::stateCommandHandler.createSetStateCommand(LED_STATE_INVALID);
    auto type = LED_COMMAND_HANDLERS::getCommandType(request);
    auto ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(request.isEmpty() && type == LED_COMMAND_INVALID && ledState == LED_STATE_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(LED_STATE_INVALID);
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledState == LED_STATE_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledState == LED_STATE_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createGetStateCommand();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(type == LED_COMMAND_STATE_GET && ledState == LED_STATE_INVALID);
}

void LedTest::testColor()
{
    //set-request, ok-response, valid colors
    for(int color = 0; color < LED_COLOR_INVALID;  ++color)
    {
        LedColor ledColor = (LedColor)color;
        QString request = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(ledColor);
        request.remove("\n");
        LedCommandType type = LED_COMMAND_HANDLERS::getCommandType(request);
        ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
        QVERIFY(type == LED_COMMAND_COLOR_SET && ledColor == color);

        request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(ledColor);
        request.remove("\n");
        type = LED_COMMAND_HANDLERS::getCommandType(request);
        ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
        QVERIFY(type == LED_COMMAND_RESPONSE_OK && ledColor == color);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(LED_COLOR_INVALID);
    auto type = LED_COMMAND_HANDLERS::getCommandType(request);
    auto ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(request.isEmpty() && type == LED_COMMAND_INVALID && ledColor == LED_COLOR_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(LED_COLOR_INVALID);
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledColor == LED_COLOR_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledColor == LED_COLOR_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createGetColorCommand();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(type == LED_COMMAND_COLOR_GET && ledColor == LED_COLOR_INVALID);
}

void LedTest::testRate()
{    
    //set-request, ok-response, valid colors
    for(LedRate rate = 0; rate < LED_RATE_MAX;  ++rate)
    {
        QString request = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(rate);
        request.remove("\n");
        LedCommandType type = LED_COMMAND_HANDLERS::getCommandType(request);
        auto ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
        QVERIFY(type == LED_COMMAND_RATE_SET && rate == ledRate);

        request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate);
        request.remove("\n");
        type = LED_COMMAND_HANDLERS::getCommandType(request);
        ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
        QVERIFY(type == LED_COMMAND_RESPONSE_OK && ledRate == rate);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(LED_RATE_INVALID);
    auto type = LED_COMMAND_HANDLERS::getCommandType(request);
    auto ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(request.isEmpty() && type == LED_COMMAND_INVALID && ledRate == LED_RATE_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(LED_RATE_INVALID);
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledRate == LED_RATE_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(type == LED_COMMAND_RESPONSE_FAILED && ledRate == LED_RATE_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createGetRateCommand();
    request.remove("\n");
    type = LED_COMMAND_HANDLERS::getCommandType(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(type == LED_COMMAND_RATE_GET && ledRate == LED_RATE_INVALID);
}

QTEST_MAIN(LedTest)
