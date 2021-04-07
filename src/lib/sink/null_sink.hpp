/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_NULL_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_NULL_SINK_HPP

#include <headcode/logger/sink.hpp>

#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A sink which consumes all events and does not do anything at all with it.
 */
class NullSink : public Sink {


public:
    /**
     * @brief   Constructor.
     */
    NullSink() : Sink{"null:"} {
    }

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
     */
    void Log_(Event const &) override {
    }
};


}


#endif
