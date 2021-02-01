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


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


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
 *              logger->SetBarrier(headcode::logger::Level::kWarning);      <-- Only warning and crits now.
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kSilence);      <-- No nothing, be silence.
 *              ...
 *              logger->SetBarrier(headcode::logger::Level::kUndefined);    <-- Defer to the parents log level.
 *
 * The default barrier of the root logger is set to kWarning. The default barrier of
 * all children is kUndefined. ==> Default: allow only critical and warning messages for all loggers.
 *
 *  This is the very first level barrier. The second barrier is at the Sink (see sink.hpp). A logger
 *  may push log messages to a number of sinks.
 */
class Logger {

    std::string name_;                        //!< @brief The name of this logger.
    std::list<std::string> ancestors_;        //!< @brief All names of all parent loggers in order.

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
     * @brief   All the ancestors of this logger in order.
     * @return  The ancestors of this logger.
     */
    [[nodiscard]] std::list<std::string> const & GetAncestors() const {
        return ancestors_;
    }

    /**
     * @brief   Gets the time point of birth of the logger subsystem.
     * @return  The time point when the logger subsystem came to live.
     */
    static std::chrono::system_clock::time_point GetBirth();

    /**
     * @brief   Gets the Logger instance by name.
     *
     * If a logger instance with the given name is not existing,
     * a new instance will be created.
     *
     * Any leading and trailing '.' in the name are dropped.
     *
     * @param   name        the name of the logger instance.
     * @return  The logger instance with that name.
     */
    static std::shared_ptr<Logger> GetLogger(std::string name = {});

    /**
     * @brief   Returns the name of this logger instance.
     * @return  The name of this logger.
     */
    [[nodiscard]] std::string GetName() const;

private:
    /**
     * @brief   Constructor
     * @param   name            the name of the logger.
     */
    explicit Logger(std::string name);
};


}


#endif
