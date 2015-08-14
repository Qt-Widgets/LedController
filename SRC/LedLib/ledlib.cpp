#include "ledlib.h"

//////////////////////////////////////
///     AbstractCommandHandler     ///
//////////////////////////////////////

AbstractCommandHandler::AbstractCommandHandler()
{
    mResponseResultTexts.append("OK");
    mResponseResultTexts.append("FAILED");
}

//////////////////////////////////////
///        StateCommandHandler     ///
//////////////////////////////////////

StateCommandHandler::StateCommandHandler()
{
    for(int commandText = 0; commandText < COMMAND_TYPE_INVALID; ++ commandText){
        mCommandTexts.append("");
    }

    mCommandTexts[COMMAND_TYPE_REQUEST] = "set-led-state";
    mCommandTexts[COMMAND_TYPE_RESPONSE] = "get-led-state";

    for(int state = 0; state < STATE_INVALID; ++ state){
        mStateTexts.append("");
    }

    mStateTexts[STATE_ON] = "on";
    mStateTexts[STATE_OFF] = "off";
}

QString StateCommandHandler::createRequest(const State state) const
{
    return QString("%1 %2\n")
            .arg(mCommandTexts[REQUEST_COMMAND])
            .arg(mStateTexts[state]);
}

QString StateCommandHandler::createResponse(const ResponseResult result, const State state) const
{
    QString response = result == RESPONSE_OK?
                       QString("%1 %2\n").arg(mResponseResultTexts[result]).arg(mStateTexts[state]) :
                       QString("%1\n").arg(mResponseResultTexts[result]);

    return response;
}

StateCommandHandler::State StateCommandHandler::getStateFromCommand(const QString commandStr, const CommandType type) const
{
    State state = STATE_INVALID;

    if(isValid(commandStr, type))
    {
        QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);
        if(type == COMMAND_TYPE_REQUEST){
            state = strToState(parts[REQUEST_STATE]);
        }
        else if(type == COMMAND_TYPE_RESPONSE && parts.size() == RESPONSE_MAX_SIZE){
            state = strToState(parts[RESPONSE_STATE]);
        }
    }

    return state;
}

bool StateCommandHandler::isValid(const QString commandStr, const CommandType type) const
{  
    bool result = true;
    QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);

    if(type == COMMAND_TYPE_REQUEST)
    {
        if(parts.size() != REQUEST_SIZE ||
           parts[REQUEST_COMMAND] != mCommandTexts[COMMAND_TYPE_REQUEST] ||
           !mStateTexts.contains(parts[REQUEST_STATE]))
        {
            result = false;
        }
    }
    else if(type == COMMAND_TYPE_RESPONSE)
    {
        if(parts.size() < RESPONSE_MIN_SIZE ||
           parts.size() > RESPONSE_MAX_SIZE ||
           !mResponseResultTexts.contains(parts[RESPONSE_RESULT]) ||
           (parts.size() == RESPONSE_MAX_SIZE && !mStateTexts.contains(parts[RESPONSE_STATE])))
        {
            result = false;
        }
    }

    return result;
}

StateCommandHandler::State StateCommandHandler::strToState(const QString stateStr) const
{
    State state = STATE_INVALID;

    if(stateStr == mStateTexts[STATE_ON]){
        state = STATE_ON;
    }
    else  if(stateStr == mStateTexts[STATE_OFF]){
     state = STATE_OFF;
    }

    return state;
}

//////////////////////////////////////
///        ColorCommandHandler     ///
//////////////////////////////////////

ColorCommandHandler::ColorCommandHandler()
{
    for(int commandText = 0; commandText < COMMAND_TYPE_INVALID; ++ commandText){
        mCommandTexts.append("");
    }

    mCommandTexts[COMMAND_TYPE_REQUEST] = "set-led-color";
    mCommandTexts[COMMAND_TYPE_RESPONSE] = "get-led-color";

    for(int state = 0; state < COLOR_INVALID; ++ state){
        mColorTexts.append("");
    }

    mColorTexts[COLOR_RED] = "red";
    mColorTexts[COLOR_GREEN] = "green";
    mColorTexts[COLOR_BLUE] = "blue";
}

QString ColorCommandHandler::createRequest(const ColorCommandHandler::Color color) const
{
    return QString("%1 %2\n")
            .arg(mCommandTexts[REQUEST_COMMAND])
            .arg(mColorTexts[color]);
}

QString ColorCommandHandler::createResponse(const ResponseResult result, const Color color) const
{
    QString response = result == RESPONSE_OK?
                       QString("%1 %2\n").arg(mResponseResultTexts[result]).arg(mColorTexts[color]) :
                       QString("%1\n").arg(mResponseResultTexts[result]);

    return response;
}

ColorCommandHandler::Color ColorCommandHandler::getColorFromCommand(const QString commandStr, const CommandType type) const
{
    Color color = COLOR_INVALID;

    if(isValid(commandStr, type))
    {
        QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);
        if(type == COMMAND_TYPE_REQUEST){
            color = strToColor(parts[REQUEST_COLOR]);
        }
        else if(type == COMMAND_TYPE_RESPONSE && parts.size() == RESPONSE_MAX_SIZE){
            color = strToColor(parts[RESPONSE_COLOR]);
        }
    }

    return color;
}

bool ColorCommandHandler::isValid(const QString commandStr, const CommandType type) const
{
    bool result = true;
    QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);

    if(type == COMMAND_TYPE_REQUEST)
    {
        if(parts.size() != REQUEST_SIZE ||
           parts[REQUEST_COMMAND] != mCommandTexts[COMMAND_TYPE_REQUEST] ||
           !mColorTexts.contains(parts[REQUEST_COLOR]))
        {
            result = false;
        }
    }
    else if(type == COMMAND_TYPE_RESPONSE)
    {
        if(parts.size() < RESPONSE_MIN_SIZE ||
           parts.size() > RESPONSE_MAX_SIZE ||
           !mResponseResultTexts.contains(parts[RESPONSE_RESULT]) ||
           (parts.size() == RESPONSE_MAX_SIZE && !mColorTexts.contains(parts[RESPONSE_COLOR])))
        {
            result = false;
        }
    }

    return result;
}

ColorCommandHandler::Color ColorCommandHandler::strToColor(const QString colorStr) const
{
    Color color = COLOR_INVALID;

    if(colorStr == mColorTexts[COLOR_RED]){
        color = COLOR_RED;
    }
    else if(colorStr == mColorTexts[COLOR_GREEN]){
        color = COLOR_GREEN;
    }
    else if(colorStr == mColorTexts[COLOR_BLUE]){
        color = COLOR_BLUE;
    }

    return color;
}

//////////////////////////////////////
///        RateCommandHandler      ///
//////////////////////////////////////

RateCommandHandler::RateCommandHandler()
{
    for(int commandText = 0; commandText < COMMAND_TYPE_INVALID; ++ commandText){
        mCommandTexts.append("");
    }

    mCommandTexts[COMMAND_TYPE_REQUEST] = "set-led-rate";
    mCommandTexts[COMMAND_TYPE_RESPONSE] = "get-led-rate";
}

QString RateCommandHandler::createRequest(const quint8 rate) const
{
    return QString("%1 %2\n")
            .arg(mCommandTexts[REQUEST_COMMAND])
            .arg(rate);
}

QString RateCommandHandler::createResponse(const ResponseResult result, const quint8 rate) const
{
    QString response = result == RESPONSE_OK?
                       QString("%1 %2\n").arg(mResponseResultTexts[result]).arg(rate) :
                       QString("%1\n").arg(mResponseResultTexts[result]);

    return response;
}

quint8 RateCommandHandler::getRateFromCommand(const QString commandStr, const AbstractCommandHandler::CommandType type, bool& ok) const
{
   quint8 rate = 0;

    ok = isValid(commandStr, type);

    if(ok)
    {
        QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);
        if(type == COMMAND_TYPE_REQUEST){
            rate = parts[REQUEST_RATE].toUShort(&ok);
        }
        else if(type == COMMAND_TYPE_RESPONSE && parts.size() == RESPONSE_MAX_SIZE){
            rate = parts[RESPONSE_RATE].toUShort(&ok);
        }

        if(ok && rate > mMaxRate){
            ok = false;
        }
    }

    return rate;
}

bool RateCommandHandler::isValid(const QString commandStr, const AbstractCommandHandler::CommandType type) const
{
    bool result = true;
    QStringList parts = commandStr.split(" ", QString::SkipEmptyParts);
    bool rateOk;

    if(type == COMMAND_TYPE_REQUEST)
    {
        if(parts.size() != REQUEST_SIZE ||
           parts[REQUEST_COMMAND] != mCommandTexts[COMMAND_TYPE_REQUEST])
        {
            result = false;
        }

        quint8 rate = parts[REQUEST_RATE].toUShort(&rateOk);
        if(!rateOk || rate > mMaxRate){
            result = false;
        }
    }
    else if(type == COMMAND_TYPE_RESPONSE)
    {
        if(parts.size() < RESPONSE_MIN_SIZE ||
           parts.size() > RESPONSE_MAX_SIZE ||
           !mResponseResultTexts.contains(parts[RESPONSE_RESULT]))
        {
            result = false;
        }

        if((parts.size() == RESPONSE_MAX_SIZE && parts[RESPONSE_RESULT] != mResponseResultTexts[RESPONSE_OK]) ||
           (parts.size() == RESPONSE_MIN_SIZE && parts[RESPONSE_RESULT] != mResponseResultTexts[RESPONSE_FAILED]))
        {
            result = false;
        }

        if(parts.size() == RESPONSE_MAX_SIZE)
        {
            quint8 rate = parts[RESPONSE_RATE].toUShort(&rateOk);
            if(!rateOk || rate > mMaxRate){
                result = false;
            }
        }
    }

    return result;
}

//////////////////////////////////////
///          CommandPool           ///
//////////////////////////////////////

QHash<CommandPool::LedCommandType, CommandPool::AbstractCommandHandlerPtr> CommandPool::mCommandStorage;

AbstractCommandHandler *CommandPool::getCommandHandler(LedCommandType type)
{
    if(type >= LED_COMMAND_INVALID){
        return nullptr;
    }

    if(!mCommandStorage.contains(type))
    {
       switch(type)
       {
       case LED_COMMAND_STATE:
           mCommandStorage.insert(type, AbstractCommandHandlerPtr( new StateCommandHandler));
           break;
       case LED_COMMAND_COLOR:
           mCommandStorage.insert(type, AbstractCommandHandlerPtr( new ColorCommandHandler));
           break;
       case LED_COMMAND_RATE:
           mCommandStorage.insert(type, AbstractCommandHandlerPtr( new RateCommandHandler));
           break;
       default:
           break;
       }
    }

    auto handlerPtr = mCommandStorage.find(type);
    return (*handlerPtr).get();
}

CommandPool::CommandInfo CommandPool::getCommandInfo(QString commandStr)
{
    CommandInfo result (LED_COMMAND_INVALID, AbstractCommandHandler::COMMAND_TYPE_INVALID);

    for(int type = 0; type < LED_COMMAND_INVALID; ++ type)
    {
        AbstractCommandHandler* handler = getCommandHandler(LedCommandType(type));
        if(handler != nullptr)
        {
            if(handler->isValid(commandStr, AbstractCommandHandler::COMMAND_TYPE_REQUEST))
            {
                result = CommandInfo(LedCommandType(type), AbstractCommandHandler::COMMAND_TYPE_REQUEST);
                break;
            }
            else if(handler->isValid(commandStr, AbstractCommandHandler::COMMAND_TYPE_RESPONSE))
            {
                result = CommandInfo(LedCommandType(type), AbstractCommandHandler::COMMAND_TYPE_RESPONSE);
                break;
            }
        }
    }

    return result;
}


void Led::test()
{

}
