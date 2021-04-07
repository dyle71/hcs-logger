/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_CONSOLE_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_CONSOLE_SINK_HPP

#include "mutex_sink.hpp"

#include <string>
#include <ostream>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   Pushes all log messages to stderr.
 */
class ConsoleSink : public MutexSink {

    std::ostream * out_{nullptr};

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     * @param   stream_url      URL of the stream to write to (either "stdout:" or "stderr:")
     */
    explicit ConsoleSink(std::string stream_url);

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
