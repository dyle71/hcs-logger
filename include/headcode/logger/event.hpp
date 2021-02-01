/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_EVENT_HPP
#define HEADCODE_SPACE_LOGGER_EVENT_HPP

#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <utility>

#include "level.hpp"


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


// fwd
class Logger;


/**
 * @brief   This is something to be logged.
 */
class Event : public std::stringstream {

    std::chrono::system_clock::time_point time_point_;        //!< @brief When the event happened.
    std::shared_ptr<Logger> logger_;                          //!< @brief The logger the event is assigned.
    int level_;                                               //!< @brief Log level value (see level.hpp)
    std::chrono::microseconds since_start_;                   //!< @brief Microseconds since start of logger subsystem

public:
    /**
     * @brief   Constructor.
     * @param   level               The log level (see level.hpp)
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Event(int level, std::string logger_name = {});

    /**
     * @brief   Constructor.
     * @param   level               The log level (see level.hpp)
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Event(Level level, std::string logger_name = {});

    /**
     * @brief   Constructor.
     * @param   level               The log level (see level.hpp)
     * @param   logger              The logger this event is addressed to.
     */
    Event(Level level, std::shared_ptr<Logger> logger);

    /**
     * @brief   Constructor.
     * @param   level               The log level (see level.hpp)
     * @param   logger              The logger this event is addressed to.
     */
    Event(int level, std::shared_ptr<Logger> logger);

    /**
     * @brief   Copy constructor.
     */
    Event(Event const &) = delete;

    /**
     * @brief   Move constructor.
     */
    Event(Event &&) = delete;

    /**
     * @brief   Destructor.
     */
    ~Event() noexcept override;

    /**
     * @brief   Assignment operator.
     */
    Event & operator=(Event const &) = delete;

    /**
     * @brief   Move operator.
     */
    Event & operator=(Event &&) = delete;

    /**
     * @brief   Gets the "age" of the event compared to the start of the log subsystem.
     * The start of the log subsystem is the very first access to any of
     * the Logger instance in the current process.
     * @return  The time passed since the logger subsystem is active.
     */
    std::chrono::microseconds GetAge() const {
        return since_start_;
    }

    /**
     * @brief   Returns the log level value.
     * @return  The log level value.
     */
    int GetLevel() const {
        return level_;
    }

    /**
     * @brief   Gets the logger this event is assigned to.
     * @return  The logger of this event.
     */
    std::shared_ptr<Logger> & GetLogger() {
        return logger_;
    }

    /**
     * @brief   Gets the logger this event is assigned to.
     * @return  The logger of this event.
     */
    std::shared_ptr<Logger> const & GetLogger() const {
        return logger_;
    }

    /**
     * @brief   Returns the created message (so far).
     * @return  The message.
     */
    std::string GetMessage() const {
        return str();
    }

    /**
     * @brief   Gets the time point when this event has been recorded.
     * @return  The time point of this event.
     */
    std::chrono::system_clock::time_point GetTimePoint() const {
        return time_point_;
    }
};


/**
 * @brief   A critical event.
 */
class Critical : public Event {

public:
    /**
     * @brief   Constructor.
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Critical(std::string logger_name = {}) : Event(Level::kCritical, std::move(logger_name)) {
    }

    /**
     * @brief   Constructor.
     * @param   logger              The logger this event is addressed to.
     */
    explicit Critical(std::shared_ptr<Logger> logger) : Event(Level::kCritical, std::move(logger)) {
    }
};


/**
 * @brief   A warning event.
 */
class Warning : public Event {

public:
    /**
     * @brief   Constructor.
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Warning(std::string logger_name = {}) : Event(Level::kWarning, std::move(logger_name)) {
    }

    /**
     * @brief   Constructor.
     * @param   logger              The logger this event is addressed to.
     */
    explicit Warning(std::shared_ptr<Logger> logger) : Event(Level::kWarning, std::move(logger)) {
    }
};


/**
 * @brief   A info event.
 */
class Info : public Event {

public:
    /**
     * @brief   Constructor.
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Info(std::string logger_name = {}) : Event(Level::kInfo, std::move(logger_name)) {
    }

    /**
     * @brief   Constructor.
     * @param   logger              The logger this event is addressed to.
     */
    explicit Info(std::shared_ptr<Logger> logger) : Event(Level::kInfo, std::move(logger)) {
    }
};


/**
 * @brief   A debug event.
 */
class Debug : public Event {

public:
    /**
     * @brief   Constructor.
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    explicit Debug(std::string logger_name = {}) : Event(Level::kDebug, std::move(logger_name)) {
    }

    /**
     * @brief   Constructor.
     * @param   logger              The logger this event is addressed to.
     */
    explicit Debug(std::shared_ptr<Logger> logger) : Event(Level::kDebug, std::move(logger)) {
    }
};


}


#endif
