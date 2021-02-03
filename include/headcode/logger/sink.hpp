/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_HPP

#include <mutex>

#include "event.hpp"
#include "level.hpp"


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


// fwd
class Formatter;


/**
 * @brief   A sink is the place all the log message go to: a file, the console, syslog...
 * A sink barrier is set to Level::kDebug on default, meaning that it would
 * pass on any log event.
 */
class Sink {

    int barrier_{static_cast<int>(Level::kDebug)};        //!< @brief Log level barrier (see description).
    std::shared_ptr<Formatter> formatter_;                //!< @brief The formatter used for this sink.
    std::uint64_t events_logged_{0};                      //!< @brief Number of events logged so far.

public:
    /**
     * @brief   Constructor
     */
    Sink();

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
    [[nodiscard]] std::shared_ptr<Formatter> GetFormatter() const {
        return formatter_;
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
     * @brief   Returns the formatter of this sink.
     * @return  The Formatter instance of this link.
     */
    void SetFormatter(std::shared_ptr<Formatter> const & formatter);

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


/**
 * @brief   A sink which consumes all events and does not do anything at all with it.
 */
class NullSink : public Sink {

private:
    /**
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] std::string GetDescription_() const override {
        return "NullSink";
    }

    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


/**
 * @brief   A sink which sends all event messages to a file.
 *
 * The file is not truncated. Log messages will be appended at the end.
 * If a filename is missing, then "a.log" will be created.
 *
 *
 * Example: log all to a file "app.log":
 *
 * @code
 * #include <fstream>
 * #include <memory>
 *
 * #include <headcode/logger/logger.hpp>
 *
 * using namespace headcode::logger;
 *
 *
 * void SetupLogging() {
 *     auto file_sink = std::make_shared<FileSink>("app.log");
 *     Logger::GetLogger()->SetSink(file_sink);
 *     Logger::GetLogger()->SetBarrier(Level::kDebug);
 * }
 *
 *
 * int main(int , char **) {
 *
 *     SetupLogging();
 *
 *     Debug() << "This is a debug message." << std::endl;
 *     Info() << "... and this is a warning message." << std::endl;
 *     Warning() << "See yourself been warned." << std::endl;
 *     Critical() << "Let's panic!" << std::endl;
 *
 *     return 0;
 * }
 * @endcode
 */
class FileSink : public Sink {

    std::string filename_;        //!< @brief The name of the file to write to.
    std::mutex mutex_;            //!< @brief mutex to write to file.

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     * @param   filename        Name of the file to write to.
     */
    explicit FileSink(std::string filename = {});

private:
    /**
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] std::string GetDescription_() const override;

    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


/**
 * @brief   Pushes all log messages to stderr.
 */
class ConsoleSink : public Sink {

    static std::mutex console_mutex_;        //!< @brief mutex to write to console.

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     */
    explicit ConsoleSink();

private:
    /**
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] std::string GetDescription_() const override;

    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


/**
 * @brief   Pushes all log messages to syslog.
 */
class SyslogSink : public Sink {

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     */
    explicit SyslogSink();

private:
    /**
     * @brief   Gets the sink description.
     * @return  A human readable description of this sink.
     */
    [[nodiscard]] std::string GetDescription_() const override;

    /**
     * @brief   This does the actual logging.
     * @param   event       the event to log.
     */
    void Log_(Event const & event) override;
};


}


#endif
