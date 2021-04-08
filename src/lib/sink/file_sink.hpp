/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_FILE_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_FILE_SINK_HPP

#include "mutex_sink.hpp"

#include <headcode/logger/sink_factory.hpp>

#include <headcode/url/url.hpp>

#include <map>
#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


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
class FileSink : public MutexSink {

    /**
     * @brief   Sink producer instance.
     */
    struct Producer : public SinkFactory::Producer {

        /**
         * @brief   Currently known sinks.
         */
        static std::map<std::string, std::shared_ptr<FileSink>> sinks;

        /**
         * @brief   Synchronizes access to sinks member.
         */
        static std::mutex mutex;

        /**
         * @brief   Creates a sink.
         * This MAY return already created objects.
         * @param   url         The URL of the sink to create.
         * @return  A sink instance.
         */
        [[nodiscard]] std::shared_ptr<Sink> Create(std::string const & url) override {

            auto parsed_url = headcode::url::URL{url}.Normalize();

            auto lock = std::unique_lock<std::mutex>(mutex);
            auto iter = sinks.find(parsed_url.GetURL());

            if (iter == sinks.end()) {
                auto sink = std::make_shared<FileSink>(parsed_url.GetURL());
                sinks.emplace(parsed_url.GetURL(), sink);
                return sink;
            }

            return iter->second;
        }

        /**
         * @brief   Returns a human readable id for the sink producer.
         * This id is also used to identify the producer within the factory.
         * @return  A description for the sink producer.
         */
        [[nodiscard]] std::string GetId() const override {
            return "FileSink Producer";
        }

        /**
         * @brief   Checks if this producer is capable to create the object.
         * @brief   url         The URL to match against.
         * @return  True, if this producer can create Sinks matching the given URL.
         */
        [[nodiscard]] bool Match(std::string const & url) const override {
            auto parsed_url = headcode::url::URL{url}.Normalize();
            return parsed_url.GetScheme() == "file";
        }
    };

    std::string filename_;        //!< @brief The name of the file to write to.

public:
    /**
     * @brief   Constructs a sink which pushes the log messages into a stream.
     * @param   file_url        URL of the file to write to.
     */
    explicit FileSink(std::string file_url);

    /**
     * @brief   Registers a Producer at the Sink Factory.
     */
    static void RegisterProducer();

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
