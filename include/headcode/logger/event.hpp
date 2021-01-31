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
#include <string>
#include <sstream>

/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   This is something to be logged.
 */
class Event : public std::stringstream {

    std::chrono::system_clock::time_point time_point_;        //!< @brief When the event happened.
    std::string logger_name_;                                 //!< @brief Name of logger the event is assigned.
    int level_;                                               //!< @brief Log level value (see level.hpp)
    std::chrono::microseconds since_start_;                   //!< @brief Microseconds since start of logger subsystem

public:
    /**
     * @brief   Constructor.
     * @param   level               The log level (see level.hpp)
     * @param   logger_name         The name of the logger this event is addressed to.
     */
    Event(int level, std::string logger_name = {});

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
    ~Event() override = default;

    /**
     * @brief   Assignment operator.
     */
    Event & operator=(Event const &) = delete;

    /**
     * @brief   Move operator.
     */
    Event & operator=(Event &&) = delete;

    /**
     * @brief   Returns the log level value.
     * @return  The log level value.
     */
    int GetLevel() const {
        return level_;
    }

    /**
     * @brief   Returns the created message (so far).
     * @return  The message.
     */
    std::string GetMessage() const {
        return str();
    }
};


}


#endif
