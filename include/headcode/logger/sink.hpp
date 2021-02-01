/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_HPP

#include <ostream>

#include "event.hpp"
#include "level.hpp"


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A sink is the place all the log message go to: a file, the console, syslog...
 * A sink barrier is set to Level::kDebug on default, meaning that it would
 * pass on any log event.
 */
class Sink {

    int barrier_{static_cast<int>(Level::kDebug)};        //!< @brief Log level barrier (see description).

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
     * @brief   Applies the sink's formatter to the event message.
     * @param   event       the event to produce a message from.
     * @return  The final string to push.
     */
    std::string Format(Event const & event);

    /**
     * @brief   Gets the log level barrier.
     *
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
     * @brief   The event to log.
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

private:
    /**
     * @brief   This does the actual logging.
     * The event passed in is valid to be logged.
     * @param   event       the event to log.
     */
    virtual void Log_(Event const & event) = 0;
};


/**
 * @brief   A sink which sends consumes all events and does not do nothing.
 */
class NullSink : public Sink {

private:
    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


/**
 * @brief   A sink which sends consumes all events and does not do nothing.
 * This is the typical sink for a file.
 *
 * TODO: Code example here.
 */
class StreamSink : public Sink {

    std::ostream & stream_;        //!< @brief The stream to push the messages to.

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     * @param   stream      the stream to push to.
     */
    explicit StreamSink(std::ostream & stream) : Sink{}, stream_{stream} {
    }

private:
    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


}


#endif
