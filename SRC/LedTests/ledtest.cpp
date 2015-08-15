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
        auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
        QVERIFY(info.type == LED_COMMAND_STATE_SET && info.param == LED_PARAM_STATE && ledState == state);

        request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(ledState);
        request.remove("\n");
        info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
        QVERIFY(info.type == LED_COMMAND_RESPONSE_OK && info.param == LED_PARAM_STATE && ledState == state);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::stateCommandHandler.createSetStateCommand(LED_STATE_INVALID);
    auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    auto ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(request.isEmpty() && info.type == LED_COMMAND_INVALID && info.param == LED_PARAM_INVALID &&  ledState == LED_STATE_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse(LED_STATE_INVALID);
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(info.type == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledState == LED_STATE_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createResponse();
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(info.type == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledState == LED_STATE_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::stateCommandHandler.createGetStateCommand();
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledState = LED_COMMAND_HANDLERS::stateCommandHandler.getState(request);
    QVERIFY(info.type == LED_COMMAND_STATE_GET && info.param == LED_PARAM_STATE && ledState == LED_STATE_INVALID);
}

void LedTest::testColor()
{
    //set-request, ok-response, valid colors
    for(int color = 0; color < LED_COLOR_INVALID;  ++color)
    {
        LedColor ledColor = (LedColor)color;
        QString request = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(ledColor);
        request.remove("\n");
        auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
        QVERIFY(info.type == LED_COMMAND_COLOR_SET && info.param == LED_PARAM_COLOR && ledColor == color);

        request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(ledColor);
        request.remove("\n");
        info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
        QVERIFY(info.type == LED_COMMAND_RESPONSE_OK && info.param == LED_PARAM_COLOR && ledColor == color);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::colorCommandHandler.createSetColorCommand(LED_COLOR_INVALID);
    auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    auto ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(request.isEmpty() && info.type  == LED_COMMAND_INVALID && info.param == LED_PARAM_INVALID  && ledColor == LED_COLOR_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse(LED_COLOR_INVALID);
    request.remove("\n");
    info  = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(info.type  == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledColor == LED_COLOR_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createResponse();
    request.remove("\n");
    info  = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(info.type == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledColor == LED_COLOR_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::colorCommandHandler.createGetColorCommand();
    request.remove("\n");
    info  = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledColor = LED_COMMAND_HANDLERS::colorCommandHandler.getColor(request);
    QVERIFY(info.type  == LED_COMMAND_COLOR_GET && info.param == LED_PARAM_COLOR && ledColor == LED_COLOR_INVALID);
}

void LedTest::testRate()
{    
    //set-request, ok-response, valid colors
    for(LedRate rate = 0; rate < LED_RATE_MAX;  ++rate)
    {
        QString request = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(rate);
        request.remove("\n");
        auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        auto ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
        QVERIFY(info.type == LED_COMMAND_RATE_SET && info.param == LED_PARAM_RATE && rate == ledRate);

        request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(rate);
        request.remove("\n");
        info = LED_COMMAND_HANDLERS::getCommandInfo(request);
        ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
        QVERIFY(info.type == LED_COMMAND_RESPONSE_OK && info.param == LED_PARAM_RATE && ledRate == rate);
    }

    //set-request, invalid state
    QString request = LED_COMMAND_HANDLERS::rateCommandHandler.createSetRateCommand(LED_RATE_INVALID);
    auto info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    auto ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(request.isEmpty() && info.type == LED_COMMAND_INVALID && info.param == LED_PARAM_INVALID && ledRate == LED_RATE_INVALID);

    //failed response, invalid state
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse(LED_RATE_INVALID);
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(info.type == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledRate == LED_RATE_INVALID);

    //failed response, no state
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createResponse();
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(info.type == LED_COMMAND_RESPONSE_FAILED && info.param == LED_PARAM_INVALID && ledRate == LED_RATE_INVALID);

    //get-request
    request = LED_COMMAND_HANDLERS::rateCommandHandler.createGetRateCommand();
    request.remove("\n");
    info = LED_COMMAND_HANDLERS::getCommandInfo(request);
    ledRate = LED_COMMAND_HANDLERS::rateCommandHandler.getRate(request);
    QVERIFY(info.type == LED_COMMAND_RATE_GET && info.param == LED_PARAM_RATE && ledRate == LED_RATE_INVALID);
}

QTEST_MAIN(LedTest)
