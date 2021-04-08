/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_HPP

#include "level.hpp"

#include <memory>
#include <string>
#include <vector>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


class Event;            //!< @brief Forward declaration of an event.
class Formatter;        //!< @brief Forward declaration of a formatter.


/**
 * @brief   A sink is the place all the log message go to: a file, the console, syslog...
 * A sink barrier is set to Level::kDebug on default, meaning that it would
 * pass on any log event.
 *
 * Sinks are identified along their URLs. Current known URLs are:
 *
 *  - "null:"                       Null sink. A sink which consumes all log events.
 *  - "stdout:"                     A console sink which writes to stdout.
 *  - "stderr:"                     A console sink which writes to stderr.
 *  - "file:///path/to/a/file"      A sink which writes into a file (add authority and path to this url if needed).
 *  - "syslog:"                     A sink which writes to syslog.
 *
 * Examples:
 * @code
 *      auto sink = headcode::log::Sink::GetSink("null:");      // A sink which consumes all events.
 * @endcode
 *
 * @code
 *      auto sink = headcode::log::Sink::GetSink("stderr:");    // A regular stderr sink.
 * @endcode
 *
 * @code
 *      auto sink = headcode::log::Sink::GetSink("stdout:");    // A sink writing to stdout sink.
 * @endcode
 *
 * @code
 *      // Creates a log file: /var/log/myapp.log.
 *      auto sink = headcode::log::Sink::GetSink("file:///var/log/myapp.log");
 * @endcode
 *
 * @code
 *      // Creates a log file: log/myapp.log (relative to current working directory).
 *      auto sink = headcode::log::Sink::GetSink("file:log/myapp.log");
 * @endcode
 *
 * @code
 *      auto sink = headcode::log::Sink::GetSink("syslog:");    // A sink pushing to syslog.
 * @endcode
 */
class Sink {

    std::string url_;                                     //!< @brief The URL of this sink.
    int barrier_{static_cast<int>(Level::kDebug)};        //!< @brief Log level barrier (see description).
    std::unique_ptr<Formatter> formatter_;                //!< @brief The formatter used for this sink.
    std::uint64_t events_logged_{0};                      //!< @brief Number of events logged so far.

public:
    /**
     * @brief   Copy constructor
     */
    Sink(Sink const &) = delete;

    /**
     * @brief   Move constructor
     */
    Sink(Sink &&) = default;

    /**
     * @brief   Destructor
     */
    virtual ~Sink();

    /**
     * @brief   Assignment operator.
     */
    Sink & operator=(Sink const &) = delete;

    /**
     * @brief   Move operator.
     */
    Sink & operator=(Sink &&) = default;

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
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] std::string GetDescription() const;

    /**
     * @brief   Returns the number of events logged so far.
     * @return  The amount of events which passed this logger instance.
     */
    [[nodiscard]] std::uint64_t GetEventsLogged() const {
        return events_logged_;
    }

    /**
     * @brief   Returns the formatter of this sink.
     * @return  The Formatter instance of this link.
     */
    [[nodiscard]] Formatter const & GetFormatter() const {
        return *(formatter_.get());
    }

    /**
     * @brief   Retrieves the URL of this sink.
     * @return  The URL of this sink.
     */
    [[nodiscard]] std::string const & GetURL() const {
        return url_;
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

    /**
     * @brief   Sets the formatter of this sink.
     * A nullptr is not accepted and refused to been applied.
     * @param   formatter       the new formatter of this sink.
     */
    void SetFormatter(std::unique_ptr<Formatter> && formatter);

protected:
    /**
     * @brief   Constructor
     * @param   url         the url by which this sink will be identified.
     */
    explicit Sink(std::string url);

    /**
     * @brief   Applies a new URL.
     * @param   url         the new URL to apply.
     */
    void SetURL(std::string url) {
        url_ = std::move(url);
    }

private:
    /**
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] virtual std::string GetDescription_() const = 0;

    /**
     * @brief   This does the actual logging.
     * The event passed in is valid to be logged.
     * @param   event       the event to log.
     */
    virtual void Log_(Event const & event) = 0;
};


}


#endif
