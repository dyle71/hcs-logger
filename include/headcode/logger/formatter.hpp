/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_FORMATTER_HPP
#define HEADCODE_SPACE_LOGGER_FORMATTER_HPP

#include <string>

#include "event.hpp"


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A formatter re-formats the message for the final log.
 * @tparam  T       the type of the log events to convert into plain log strings.
 */
class Formatter {

public:
    /**
     * @brief   Constructor
     */
    Formatter() = default;

    /**
     * @brief   Copy constructor
     */
    Formatter(Formatter const &) = default;

    /**
     * @brief   Move constructor
     */
    Formatter(Formatter &&) = default;

    /**
     * @brief   Destructor
     */
    virtual ~Formatter() = default;

    /**
     * @brief   Assignment operator.
     */
    Formatter & operator=(Formatter const &) = default;

    /**
     * @brief   Move operator.
     */
    Formatter & operator=(Formatter &&) = default;

    /**
     * @brief   Formats the log event of type T to produce the final log string.
     * @param   event       the log event to format.
     * @return  A string drawn from that log event.
     */
    std::string Format(Event const & event) {
        return Format_(event);
    }

private:
    /**
     * @brief   The detailed formatter function to reimplement in derived classes.
     * @param   event           the log event data.
     * @return  The string to push to the Sink instance.
     */
    virtual std::string Format_(Event const & event) = 0;
};


/**
 * @brief   A formatter which pushes the plain string of the event.
 */
class StringFormatter : public Formatter {

private:
    /**
     * @brief   The detailed formatter function to reimplement in derived classes.
     * @param   event           the log event data.
     * @return  The string to push to the Sink instance.
     */
    std::string Format_(Event const & event) override {
        return event.GetMessage();
    }
};


}


#endif
