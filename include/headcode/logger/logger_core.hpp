/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_LOGGER_CORE_HPP
#define HEADCODE_SPACE_LOGGER_LOGGER_CORE_HPP

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "level.hpp"


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


class Event;        //!< @brief Forward declaration of an event.
class Sink;         //!< @brief Forward declaration of a sink.


/**
 * @brief   This is the main core Logger class.
 *
 * Logger objects have a parent-child relationship based on their name. The name
 * is dot ('.') delimited and children always appending their name. The top most
 * parent of all loggers is the root logger ("<root>").
 *
 *  E.g.        Child name:     "foo.bar.baz"
 *              Parent:         "foo.bar"
 *              Grandparent:    "foo"
 *              Root:           ""      ==> "<root>".
 *
 * Each logger has an event barrier and forwards only log events (see event.hpp)
 * which log level is *less or equal* to the barrier.
 *
 * E.g.         GetBarrier() ==> headcode::logger::Level::kWarning
 *              Log level: kCritical ==> Yes
 *              Log level: kWarning ==> Yes
 *              Log level: kInfo ==> No
 *              Log level: kDebug ==> No
 *
 * However, there is also kSilence and kUndefined as log "levels". kSilence will never get
 * passed by a log level barrier, whereas kUndefined will always pass.
 *
 * The log level barrier of any logger but the root logger may be set to any log level value.
 * If kUndefined is set, then the barrier of the parent is used (recursively). The root logger
 * does not accept kUndefined.
 *
 * E.g.         auto logger = headcode::logger::Logger::GetLogger("foo");
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kDebug);        <-- Turn on every message.
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kWarning);      <-- Only warning and critical now.
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kSilence);      <-- No nothing, be silence.
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kUndefined);    <-- Defer to the parents log level.
 *
 * The default barrier of the root logger is set to kWarning. The default barrier of
 * all children is kUndefined. ==> Default: allow only critical and warning messages for all loggers.
 *
 *  This is the very first level barrier. The second barrier is at the Sink (see sink.hpp). A logger
 *  may push log messages to a number of sinks. If there is no Sink defined on a Logger instance, then
 *  all the sinks of the parent logger are used.
 *
 *  The root logger has the ConsoleSink as default.
 */
class Logger {

    std::string name_;                              //!< @brief The name of this logger.
    std::list<std::string> ancestors_;              //!< @brief All names of all parent loggers in order.
    unsigned int id_{0};                            //!< @brief An id of this logger.
    int barrier_{0};                                //!< @brief Log level barrier (see description).
    std::vector<std::weak_ptr<Sink>> sinks_;        //!< @brief URLs of all Sinks attached to this logger.
    std::uint64_t events_logged_{0};                //!< @brief Number of events logged so far.

public:
    /**
     * @brief   Copy constructor
     */
    Logger(Logger const &) = delete;

    /**
     * @brief   Move constructor
     */
    Logger(Logger &&) = delete;

    /**
     * @brief   Destructor
     */
    virtual ~Logger() = default;

    /**
     * @brief   Assignment operator.
     */
    Logger & operator=(Logger const &) = delete;

    /**
     * @brief   Move operator.
     */
    Logger & operator=(Logger &&) = delete;

    /**
     * @brief   Adds a sink to the sinks of this logger.
     * Avoids double adding.
     * @param   sink        sink to add.
     */
    void AddSink(std::shared_ptr<Sink> sink);

    /**
     * @brief   All the ancestors of this logger in order.
     * @return  The ancestors of this logger.
     */
    [[nodiscard]] std::list<std::string> const & GetAncestors() const {
        return ancestors_;
    }

    /**
     * @brief   Gets the log level barrier.
     *
     * -1 ==> Forward to parent Logger.
     *  0 ==> Silently drop.
     *  1 ==> Only pass Critical events.
     *  2 ==> Pass Critical and Warning events.
     *  3 ==> Pass Critical, Warning, and Info events.
     *  4 ==> Pass Critical, Warning, Info, and Debug events.
     *
     * See Level enumeration in level.hpp.
     *
     * In general any event with a level 0 > value <= this barrier is passed.
     * Users are free to define their own log levels and raise the barriers
     * accordingly.
     *
     * @return  The current log level barrier event levels need to pass.
     */
    [[nodiscard]] int GetBarrier() const {
        return barrier_;
    }

    /**
     * @brief   Gets the time point of birth of the logger subsystem.
     * @return  The time point when the logger subsystem came to live.
     */
    static std::chrono::system_clock::time_point GetBirth();

    /**
     * @brief   Returns the number of events logged so far.
     * @return  The amount of events which passed this logger instance.
     */
    [[nodiscard]] std::uint64_t GetEventsLogged() const {
        return events_logged_;
    }

    /**
     * @brief   Gets the ID of this logger.
     * @return  The id ID of this logger.
     */
    [[nodiscard]] unsigned int GetId() const {
        return id_;
    }

    /**
     * @brief   Gets the Logger instance by name.
     *
     * If a logger instance with the given name is not existing,
     * a new instance will be created.
     *
     * Any leading and trailing '.' in the name are dropped.
     * There is always the top most root logger with an empty name.
     *
     * @param   name        the name of the logger instance.
     * @return  The logger instance with that name.
     */
    static Logger * GetLogger(std::string name = {});

    /**
     * @brief   Retrieves a list of all known loggers.
     * @return  A list of all registered loggers.
     */
    [[nodiscard]] static std::list<std::string> GetLoggers();

    /**
     * @brief   Gets the next known parent logger.
     * @return  The next known, registered parent logger.
     */
    [[nodiscard]] Logger * GetParentLogger() const;

    /**
     * @brief   Returns the name of this logger instance.
     * @return  The name of this logger.
     */
    [[nodiscard]] std::string GetName() const;

    /**
     * @brief   Gets all the sinks associated with this logger.
     * @return  All the sinks of this logger.
     */
    [[nodiscard]] std::vector<std::weak_ptr<Sink>> const & GetSinks() const {
        return sinks_;
    }

    /**
     * @brief   Checks if this logger is the root logger.
     * @return  True, if it is.
     */
    [[nodiscard]] bool IsRootLogger() const {
        return name_.empty();
    }

    /**
     * @brief   Logs an event.
     * This is the real logging procedure.
     * @param   event       the event to log.
     */
    void Log(Event const & event);

    /**
     * @brief   Sets a new log level barrier (see description of GetBarrier()).
     * @param   barrier     the new log level barrier for events.
     */
    void SetBarrier(int barrier);

    /**
     * @brief   Sets a new log level barrier (see description of GetBarrier()).
     * @param   barrier     the new log level barrier for events.
     */
    void SetBarrier(Level barrier);

    /**
     * @brief   Sets a single Sink.
     * This removes any previous sinks at this logger.
     * @param   sink        the sink to set.
     */
    void SetSink(std::shared_ptr<Sink> sink);

private:
    /**
     * @brief   Constructor
     * @param   name            the name of the logger.
     */
    explicit Logger(std::string name, unsigned int id);

    /**
     * @brief   Pushed the given event to a sink.
     * The event has already passed the barrier.
     * @param   event       the event to push.
     */
    void Push(Event const & event);
};


}


#endif
