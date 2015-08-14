#ifndef LEDLIB_H
#define LEDLIB_H

#ifdef _WIN32
   #define EXPORT __declspec( dllexport )
#else
   #define EXPORT
#endif

#include <QHash>
#include <QString>
#include <QStringList>
#include <QDebug>

#include <memory>

class CommandPool;

//////////////////////////////////////
///     AbstractCommandHandler     ///
//////////////////////////////////////

class EXPORT AbstractCommandHandler
{
    friend class CommandPool;

public:
    enum CommandType{
        COMMAND_TYPE_REQUEST,
        COMMAND_TYPE_RESPONSE,
        COMMAND_TYPE_INVALID
    };

    enum ResponseResult{
        RESPONSE_OK,
        RESPONSE_FAILED,
        RESPONSE_INVALID
    };

    virtual ~AbstractCommandHandler(){}
    virtual bool isValid(const QString commandStr, const CommandType type) const = 0;

protected:
    QStringList mCommandTexts;
    QStringList mResponseResultTexts;

    AbstractCommandHandler();  
};

//////////////////////////////////////
///        StateCommandHandler     ///
//////////////////////////////////////

class EXPORT StateCommandHandler : public AbstractCommandHandler
{
   friend class CommandPool;

public:
    enum State {
        STATE_ON,
        STATE_OFF,
        STATE_INVALID
    };

    QString createRequest(const State state) const;
    QString createResponse(const ResponseResult result, const State state = STATE_INVALID) const;
    State getStateFromCommand(const QString commandStr, const CommandType type) const;

private:
    enum RequestStructure {
        REQUEST_COMMAND,
        REQUEST_STATE,
        REQUEST_SIZE
    };

    enum ResponseStructure{
        RESPONSE_RESULT,
        RESPONSE_STATE,
        RESPONSE_MIN_SIZE = 1,
        RESPONSE_MAX_SIZE = 2
    };

    QStringList mStateTexts;

    StateCommandHandler();
    bool isValid(const QString commandStr, const CommandType type) const;
    State strToState(const QString stateStr) const ;
};

//////////////////////////////////////
///        ColorCommandHandler     ///
//////////////////////////////////////

class EXPORT ColorCommandHandler : public AbstractCommandHandler
{
   friend class CommandPool;

public:
    enum Color{
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_INVALID
    };

    QString createRequest(const Color color) const;
    QString createResponse(const ResponseResult result, const Color color = COLOR_INVALID) const;
    Color getColorFromCommand(const QString commandStr, const CommandType type) const;

private:
    enum RequestStructure{
        REQUEST_COMMAND,
        REQUEST_COLOR,
        REQUEST_SIZE
    };

    enum ResponseStructure{
        RESPONSE_RESULT,
        RESPONSE_COLOR,
        RESPONSE_MIN_SIZE = 1,
        RESPONSE_MAX_SIZE = 2
    };

    QStringList mColorTexts;

    ColorCommandHandler();
    bool isValid(const QString commandStr, const CommandType type) const ;
    Color strToColor(const QString colorStr) const ;
};

//////////////////////////////////////
///        RateCommandHandler      ///
//////////////////////////////////////

class EXPORT RateCommandHandler : public AbstractCommandHandler
{
    friend class CommandPool;

public:
    QString createRequest(const quint8 rate) const;
    QString createResponse(const ResponseResult result, const quint8 rate = 0) const;
    quint8 getRateFromCommand(const QString commandStr, const CommandType type, bool& ok) const;

private:
    enum RequestStructure{
        REQUEST_COMMAND,
        REQUEST_RATE,
        REQUEST_SIZE
    };

    enum ResponseStructure{
        RESPONSE_RESULT,
        RESPONSE_RATE,
        RESPONSE_MIN_SIZE = 1,
        RESPONSE_MAX_SIZE = 2
    };

    static const int mMaxRate = 5;

    RateCommandHandler();
    bool isValid(const QString commandStr, const CommandType type) const;
};

//////////////////////////////////////
///          CommandPool           ///
//////////////////////////////////////

class EXPORT CommandPool
{
public:
    enum LedCommandType
    {
        LED_COMMAND_STATE,
        LED_COMMAND_COLOR,
        LED_COMMAND_RATE,

        LED_COMMAND_INVALID
    };

    typedef QPair<CommandPool::LedCommandType, AbstractCommandHandler::CommandType> CommandInfo;

   static AbstractCommandHandler* getCommandHandler (LedCommandType type);
   static CommandInfo getCommandInfo (QString commandStr);

private:
   typedef std::shared_ptr<AbstractCommandHandler> AbstractCommandHandlerPtr;
   static QHash<LedCommandType, AbstractCommandHandlerPtr> mCommandStorage;
};

//////////////////////////////////////
///             Led                ///
//////////////////////////////////////

class EXPORT Led
{
public:
    StateCommandHandler::State state;
    ColorCommandHandler::Color color;
    quint8 rate;

    void test();
};

#endif // LEDLIB_H
