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

#include <headcode/logger/sink_factory.hpp>

#include <headcode/url/url.hpp>

#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A sink which consumes all events and does not do anything at all with it.
 */
class NullSink : public Sink {

    /**
     * @brief   Sink producer instance.
     */
    struct Producer : public SinkFactory::Producer {

        /**
         * @brief   Creates a sink.
         * @param   url         The URL of the sink to create.
         * @return  A sink instance.
         */
        [[nodiscard]] std::shared_ptr<Sink> Create(std::string const & url) override {

            auto parsed_url = headcode::url::URL{url}.Normalize();
            if (parsed_url.GetScheme() == "null") {
                return std::make_shared<NullSink>();
            }
            return nullptr;
        }

        /**
         * @brief   Returns a human readable id for the sink producer.
         * This id is also used to identify the producer within the factory.
         * @return  A description for the sink producer.
         */
        [[nodiscard]] std::string GetId() const override {
            return "NullSink Producer";
        }

        /**
         * @brief   Checks if this producer is capable to create the object.
         * @brief   url         The URL to match against.
         * @return  True, if this producer can create Sinks matching the given URL.
         */
        [[nodiscard]] bool Match(std::string const & url) const override {
            auto parsed_url = headcode::url::URL{url}.Normalize();
            return parsed_url.GetScheme() == "null";
        }
    };

public:
    /**
     * @brief   Constructor.
     */
    NullSink() : Sink{"null:"} {
    }

    /**
     * @brief   Registers a Producer at the Sink Factory.
     */
    static void RegisterProducer();

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
