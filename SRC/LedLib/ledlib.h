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

namespace LEDGLOBAL
{
    enum LedParameter
    {
        LED_PARAM_STATE,
        LED_PARAM_COLOR,
        LED_PARAM_RATE,
        LED_PARAM_INVALID
    };

    enum LedCommandType{
      LED_COMMAND_STATE_SET,
      LED_COMMAND_STATE_GET,
      LED_COMMAND_COLOR_SET,
      LED_COMMAND_COLOR_GET,
      LED_COMMAND_RATE_SET,
      LED_COMMAND_RATE_GET,
      LED_COMMAND_RESPONSE_OK,
      LED_COMMAND_RESPONSE_FAILED,
      LED_COMMAND_INVALID
    };

    enum LedState{
        LED_STATE_ON,
        LED_STATE_OFF,
        LED_STATE_INVALID
    };

    enum LedColor{
        LED_COLOR_RED,
        LED_COLOR_GREEN,
        LED_COLOR_BLUE,
        LED_COLOR_INVALID
    };

   typedef quint8 LedRate;
   static const int LED_RATE_MAX = 5;
   static const int LED_RATE_INVALID = LED_RATE_MAX + 1;

   //////////////////////////////////////
   ///              Led               ///
   //////////////////////////////////////

   /**
   *  Simple led simulation
   */

   class EXPORT Led
   {
       LedColor mColor;
       LedState mState;
       LedRate mRate;

   public:
       Led();

       bool setLedColor(const LedColor color);
       bool setLedState(const LedState state);
       bool setLedRate(const LedRate rate);

       LedColor getLedColor() const;
       LedState getLedState() const;
       LedRate getLedRate() const;
   };

    //////////////////////////////////////
    ///     AbstractCommandHandler     ///
    //////////////////////////////////////

   /**
   *  A base for other commands
   */

    class EXPORT AbstractCommandHandler
    {
        friend class LED_COMMAND_HANDLERS;

    protected:
       QHash<LedCommandType, QString> mCommands;
       QHash<LedCommandType, QString> mResponses;

       enum LedCommandStructure{
           LED_STRUCT_POS_COMMAND,
           LED_STRUCT_POS_ARGUMENT,

           LED_STRUCT_SIZE_NO_ARGUMENT = 1,
           LED_STRUCT_SIZE_WITH_ARGUMENT = 2};

    public:
       virtual LedCommandType getType(const QString command) const;

    protected:
        explicit AbstractCommandHandler();
    };


    //////////////////////////////////////
    ///        StateCommandHandler     ///
    //////////////////////////////////////

    /**
    *  Creates and handles state related commands
    */

    class EXPORT StateCommandHandler : public AbstractCommandHandler
    {
       friend class LED_COMMAND_HANDLERS;

    private:
       QHash<LedState, QString> mStates;

    public:
       QString createSetStateCommand(const LedState state) const;
       QString createGetStateCommand() const;
       QString createResponse(LedState state = LED_STATE_INVALID);

       LedCommandType getType(const QString command) const;
       LedState getState(const QString command) const;
       LedState strToState(const QString stateStr) const;
       QString stateToStr(const LedState state) const;

     private:
       StateCommandHandler();      
    };

    //////////////////////////////////////
    ///        ColorCommandHandler     ///
    //////////////////////////////////////

    /**
    *  Creates and handles color related commands
    */

    class EXPORT ColorCommandHandler : public AbstractCommandHandler
    {
       friend class LED_COMMAND_HANDLERS;

    private:
       QHash<LedColor, QString> mColors;

    public:
       QString createSetColorCommand(const LedColor color) const;
       QString createGetColorCommand() const;
       QString createResponse(LedColor color = LED_COLOR_INVALID);

       LedCommandType getType(const QString command) const;
       LedColor getColor(const QString command) const;
       LedColor strToColor(const QString colorStr) const;
       QString colorToStr (const LedColor color) const;

    private:
       explicit ColorCommandHandler();       
    };

    //////////////////////////////////////
    ///         RateCommandHandler     ///
    //////////////////////////////////////

    /**
    *  Creates and handles rate related commands
    */

    class EXPORT RateCommandHandler : public AbstractCommandHandler
    {
        friend class LED_COMMAND_HANDLERS;

    public:
       QString createSetRateCommand(const LedRate rate) const;
       QString createGetRateCommand() const;
       QString createResponse(LedRate rate = LED_RATE_MAX + 1);

       LedCommandType getType(const QString command) const;
       LedRate getRate(const QString command) const;

    private:
        explicit RateCommandHandler();
    };

    //////////////////////////////////////
    ///      LED_COMMAND_HANDLERS      ///
    //////////////////////////////////////

    /**
    *  Provides the means to access handlers from other classes
    */

    struct CommandInfo
    {
        LedCommandType type;
        LedParameter param;

        CommandInfo();
    };

    class EXPORT LED_COMMAND_HANDLERS
    {
    public:
        static StateCommandHandler stateCommandHandler;
        static ColorCommandHandler colorCommandHandler;
        static RateCommandHandler rateCommandHandler;

        static CommandInfo getCommandInfo(QString command);
        static QStringList getCommandList(QString commandLine); // splits several commands merged in one string
    };
}

#endif // LEDLIB_H

