/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_SYSLOG_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_SYSLOG_SINK_HPP

#include <headcode/logger/sink.hpp>

#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


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
