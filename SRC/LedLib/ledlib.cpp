#include "ledlib.h"

using namespace LEDGLOBAL;


//////////////////////////////////////
///              Led               ///
//////////////////////////////////////

Led::Led() : mColor(LED_COLOR_INVALID), mState(LED_STATE_INVALID), mRate(LED_RATE_INVALID)
{

}

void Led::setLedColor(const LedColor color)
{
    mColor = color;
}

void Led::setLedState(const LedState state)
{
   mState = state;
}

void Led::setLedRate(const LedRate rate)
{
    mRate = rate;
}

LedColor Led::getLedColor() const
{
    return mColor;
}

LedState Led::getLedState() const
{
    return mState;
}

LedRate Led::getLedRate() const
{
    return mRate;
}

//////////////////////////////////////
///     AbstractCommandHandler     ///
//////////////////////////////////////

AbstractCommandHandler::AbstractCommandHandler()
{
    mResponses.insert(LED_COMMAND_RESPONSE_OK, "OK");
    mResponses.insert(LED_COMMAND_RESPONSE_FAILED, "FAILED");
}

LedCommandType AbstractCommandHandler::getType(const QString command) const
{
    LedCommandType type = LED_COMMAND_INVALID;

    if(command.isEmpty()){
        return type;
    }

    QStringList parts = command.split(" ", QString::SkipEmptyParts);

    if(parts[LED_STRUCT_POS_COMMAND] == mResponses[LED_COMMAND_RESPONSE_OK] && //if OK is matched
       parts.size() == LED_STRUCT_SIZE_WITH_ARGUMENT) //validating size
    {
        type = LED_COMMAND_RESPONSE_OK;
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mResponses[LED_COMMAND_RESPONSE_FAILED] && //if FAILED is matched
            parts.size() == LED_STRUCT_SIZE_NO_ARGUMENT) //validating size
    {
        type = LED_COMMAND_RESPONSE_FAILED;
    }

    return type;
}

//////////////////////////////////////
///        StateCommandHandler     ///
//////////////////////////////////////

StateCommandHandler::StateCommandHandler()
{
    mStates.insert(LED_STATE_ON, "on");
    mStates.insert(LED_STATE_OFF, "off");

    mCommands.insert(LED_COMMAND_STATE_SET, "set-led-state");
    mCommands.insert(LED_COMMAND_STATE_GET, "get-led-state");
}

QString StateCommandHandler::createSetStateCommand(const LedState state) const
{
    QString command;

    if(mStates.contains(state)){
        command = QString("%1 %2\n")
                .arg(mCommands[LED_COMMAND_STATE_SET])
                .arg(mStates[state]);
    }

    return command;
}

QString StateCommandHandler::createGetStateCommand() const
{
    return QString("%1\n").arg(mCommands[LED_COMMAND_STATE_GET]);
}

QString StateCommandHandler::createResponse(LedState state)
{
    QString command;

    if(!mStates.contains(state)){
        state = LED_STATE_INVALID;
    }

    command = (state == LED_STATE_INVALID)?
                QString("%1\n").arg(mResponses[LED_COMMAND_RESPONSE_FAILED]) :
                QString("%1 %2\n").arg(mResponses[LED_COMMAND_RESPONSE_OK]).arg(mStates[state]);

    return command;
}

LedCommandType StateCommandHandler::getType(const QString command) const
{
    LedCommandType type = AbstractCommandHandler::getType(command);
    if(command.isEmpty()){
        return type;
    }

    QStringList parts = command.split(" ", QString::SkipEmptyParts);

    if(type == LED_COMMAND_RESPONSE_OK && //if the type has been determined by AbstractCommandHandler
       strToState(parts[LED_STRUCT_POS_ARGUMENT]) == LED_STATE_INVALID) //validating argument
    {
       type = LED_COMMAND_INVALID;
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_STATE_SET])
    {
        if(parts.size() == LED_STRUCT_SIZE_WITH_ARGUMENT && //validating size
                strToState(parts[LED_STRUCT_POS_ARGUMENT]) != LED_STATE_INVALID) //validating argument
        {
            type = LED_COMMAND_STATE_SET;
        }
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_STATE_GET])
    {
        if(parts.size() == LED_STRUCT_SIZE_NO_ARGUMENT){ //validating size
            type = LED_COMMAND_STATE_GET;
        }
    }

    return type;
}

LedState StateCommandHandler::getState(const QString command) const
{
   LedState state = LED_STATE_INVALID;
   LedCommandType type = getType(command);

   if(type == LED_COMMAND_STATE_SET || type == LED_COMMAND_RESPONSE_OK)
   {
      QStringList parts = command.split(" ", QString::SkipEmptyParts);
      state = strToState(parts[LED_STRUCT_POS_ARGUMENT]);
   }

   return state;
}

LedState StateCommandHandler::strToState(const QString stateStr) const
{
    LedState state = LED_STATE_INVALID;

    for(auto stateIter = mStates.begin(); stateIter != mStates.end(); ++stateIter)
    {
        if(*stateIter == stateStr)
        {
            state = stateIter.key();
            break;
        }
    }

    return state;
}

//////////////////////////////////////
///        ColorCommandHandler     ///
//////////////////////////////////////

ColorCommandHandler::ColorCommandHandler()
{
    mColors.insert(LED_COLOR_RED, "red");
    mColors.insert(LED_COLOR_GREEN, "green");
    mColors.insert(LED_COLOR_BLUE, "blue");

    mCommands.insert(LED_COMMAND_COLOR_SET, "set-led-color");
    mCommands.insert(LED_COMMAND_COLOR_GET, "get-led-color");
}

QString ColorCommandHandler::createSetColorCommand(const LedColor color) const
{
    QString command;

    if(mColors.contains(color)){
        command = QString("%1 %2\n")
                .arg(mCommands[LED_COMMAND_COLOR_SET])
                .arg(mColors[color]);
    }

    return command;
}

QString ColorCommandHandler::createGetColorCommand() const
{
    return QString("%1\n").arg(mCommands[LED_COMMAND_COLOR_GET]);
}

QString ColorCommandHandler::createResponse(LedColor color)
{
    QString command;

    if(!mColors.contains(color)){
        color = LED_COLOR_INVALID;
    }

    command = (color == LED_COLOR_INVALID)?
                QString("%1\n").arg(mResponses[LED_COMMAND_RESPONSE_FAILED]) :
                QString("%1 %2\n").arg(mResponses[LED_COMMAND_RESPONSE_OK]).arg(mColors[color]);

    return command;
}

LedCommandType ColorCommandHandler::getType(const QString command) const
{
    LedCommandType type = AbstractCommandHandler::getType(command);
    if(command.isEmpty()){
        return type;
    }

    QStringList parts = command.split(" ", QString::SkipEmptyParts);

    if(type == LED_COMMAND_RESPONSE_OK && //if the type has been determined by AbstractCommandHandler
       strToColor(parts[LED_STRUCT_POS_ARGUMENT]) == LED_COLOR_INVALID) //validating argument
    {
       type = LED_COMMAND_INVALID;
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_COLOR_SET])
    {
        if(parts.size() == LED_STRUCT_SIZE_WITH_ARGUMENT && //validating size
                strToColor(parts[LED_STRUCT_POS_ARGUMENT]) != LED_COLOR_INVALID) //validating argument
        {
            type = LED_COMMAND_COLOR_SET;
        }
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_COLOR_GET])
    {
        if(parts.size() == LED_STRUCT_SIZE_NO_ARGUMENT){ //validating size
            type = LED_COMMAND_COLOR_GET;
        }
    }

    return type;
}

LedColor ColorCommandHandler::getColor(const QString command) const
{
    LedColor color = LED_COLOR_INVALID;
    LedCommandType type = getType(command);

    if(type == LED_COMMAND_COLOR_SET || type == LED_COMMAND_RESPONSE_OK)
    {
       QStringList parts = command.split(" ", QString::SkipEmptyParts);
       color = strToColor(parts[LED_STRUCT_POS_ARGUMENT]);
    }

    return color;
}

LedColor ColorCommandHandler::strToColor(const QString colorStr) const
{
    LedColor color = LED_COLOR_INVALID;

    for(auto colorIter = mColors.begin(); colorIter != mColors.end(); ++colorIter)
    {
        if(*colorIter == colorStr)
        {
            color = colorIter.key();
            break;
        }
    }

    return color;
}

//////////////////////////////////////
///      RateCommandHandler        ///
//////////////////////////////////////

RateCommandHandler::RateCommandHandler()
{
    mCommands.insert(LED_COMMAND_RATE_SET, "set-led-rate");
    mCommands.insert(LED_COMMAND_RATE_GET, "get-led-rate");
}

QString RateCommandHandler::createSetRateCommand(const LedRate rate) const
{
    QString command;

    if(rate >= 0 && rate <= LED_RATE_MAX){
        command = QString("%1 %2\n")
                .arg(mCommands[LED_COMMAND_RATE_SET])
                .arg(rate);
    }

    return command;
}

QString RateCommandHandler::createGetRateCommand() const
{
    return QString("%1\n").arg(mCommands[LED_COMMAND_RATE_GET]);
}

QString RateCommandHandler::createResponse(LedRate rate)
{
    QString command;

     if(rate < 0 && rate > LED_RATE_MAX){
        rate = LED_RATE_INVALID;
    }

    command = (rate == LED_RATE_INVALID)?
                QString("%1\n").arg(mResponses[LED_COMMAND_RESPONSE_FAILED]) :
                QString("%1 %2\n").arg(mResponses[LED_COMMAND_RESPONSE_OK]).arg(rate);

    return command;
}

LedCommandType RateCommandHandler::getType(const QString command) const
{
    LedCommandType type = AbstractCommandHandler::getType(command);
    if(command.isEmpty()){
        return type;
    }

    QStringList parts = command.split(" ", QString::SkipEmptyParts);

    if(type == LED_COMMAND_RESPONSE_OK)
    {
        bool ok;
        LedRate rate = parts[LED_STRUCT_POS_ARGUMENT].toUShort(&ok);
        if(!ok || rate > LED_RATE_MAX){
            type = LED_COMMAND_INVALID;
        }
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_RATE_SET])
    {
        if(parts.size() == LED_STRUCT_SIZE_WITH_ARGUMENT)
        {
            bool ok;
            LedRate rate = parts[LED_STRUCT_POS_ARGUMENT].toUShort(&ok);
            if(ok || rate <= LED_RATE_MAX){
                type = LED_COMMAND_RATE_SET;
            }
        }
    }
    else if(parts[LED_STRUCT_POS_COMMAND] == mCommands[LED_COMMAND_RATE_GET])
    {
        if(parts.size() == LED_STRUCT_SIZE_NO_ARGUMENT){ //validating size
            type = LED_COMMAND_RATE_GET;
        }
    }

    return type;
}

LedRate RateCommandHandler::getRate(const QString command) const
{
    LedRate rate = LED_RATE_INVALID;
    LedCommandType type = getType(command);

    if(type == LED_COMMAND_RATE_SET || type == LED_COMMAND_RESPONSE_OK)
    {
        QStringList parts = command.split(" ", QString::SkipEmptyParts);

        bool ok;
        rate = parts[LED_STRUCT_POS_ARGUMENT].toUShort(&ok);

        if(!ok || rate > LED_RATE_MAX){
            rate = LED_RATE_INVALID;
        }
    }

    return rate;
}

//////////////////////////////////////
///          COMMAND_HANDLERS      ///
//////////////////////////////////////


CommandInfo::CommandInfo() : type(LED_COMMAND_INVALID), param(LED_PARAM_INVALID)
{

}

StateCommandHandler LED_COMMAND_HANDLERS::stateCommandHandler;
ColorCommandHandler LED_COMMAND_HANDLERS::colorCommandHandler;
RateCommandHandler LED_COMMAND_HANDLERS::rateCommandHandler;

CommandInfo LED_COMMAND_HANDLERS::getCommandInfo(QString command)
{
    CommandInfo info;
    LedCommandType type = LED_COMMAND_INVALID;

    type = stateCommandHandler.getType(command);
    if(type != LED_COMMAND_INVALID)
    {
        info.type = type;
        info.param = type != LED_COMMAND_RESPONSE_FAILED? LED_PARAM_STATE : LED_PARAM_INVALID;
        return info;
    }

    type = colorCommandHandler.getType(command);
    if(type != LED_COMMAND_INVALID)
    {
        info.type = type;
        info.param = type != LED_COMMAND_RESPONSE_FAILED? LED_PARAM_COLOR : LED_PARAM_INVALID;
        return info;
    }

    type = rateCommandHandler.getType(command);
    if(type != LED_COMMAND_INVALID)
    {
        info.type = type;
        info.param = type != LED_COMMAND_RESPONSE_FAILED? LED_PARAM_RATE : LED_PARAM_INVALID;
        return info;
    }

    return info;
}
