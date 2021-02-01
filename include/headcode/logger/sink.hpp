/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_HPP

#include "event.hpp"


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
    Sink(Sink const &) = delete;

    /**
     * @brief   Move constructor
     */
    Sink(Sink &&) = delete;

    /**
     * @brief   Destructor
     */
    virtual ~Sink() = default;

    /**
     * @brief   Assignment operator.
     */
    Sink & operator=(Sink const &) = delete;

    /**
     * @brief   Move operator.
     */
    Sink & operator=(Sink &&) = delete;

    /**
     * @brief   The event to log.
     * @param   event       the event to log.
     */
    void Log(Event const & event);
};


}


#endif
