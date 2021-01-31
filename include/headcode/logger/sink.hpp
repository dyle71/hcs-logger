/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_HPP


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A sink is the place all the log message go to.
 */
class Sink {

public:
    /**
     * @brief   Constructor
     */
    Sink() = default;

    /**
     * @brief   Copy constructor
     */
    Sink(Logger const &) = delete;

    /**
     * @brief   Move constructor
     */
    Sink(Logger &&) = delete;

    /**
     * @brief   Destructor
     */
    virtual ~Sink() = default;

    /**
     * @brief   Assignment operator.
     */
    Sink & operator=(Logger const &) = delete;

    /**
     * @brief   Move operator.
     */
    Sink & operator=(Logger &&) = delete;

    /**
     * @brief   Performs the logging with a message.
     * @param   message     The message to Log.
     */
    void Log(std::string message) const;
};


}


#endif
