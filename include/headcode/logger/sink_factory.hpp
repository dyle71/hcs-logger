/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_FACTORY_HPP
#define HEADCODE_SPACE_LOGGER_SINK_FACTORY_HPP

#include <memory>
#include <string>
#include <vector>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


class Sink;        //!< @brief Forward declaration of sinks.


/**
 * @brief   A factory capable to produce arbitrary sink objects.
 *
 * Any Sink is created with the help of this factory. The factory itself
 * maintains a set of sink producers. On request of a specific URL the
 * registered producers are asked if they can produce a Sink based
 * on the given URL.
 */
struct SinkFactory {

    /**
     * @brief   Each concrete Sink declares a dedicated Producer.
     *
     * The factory does not manage Sink instances but producer instances,
     * which in turn create new sink objects on demand.
     * Every sink has to declare its (proper) Producer class.
     */
    struct Producer {

        /**
         * @brief   Creates a sink.
         * This MAY return already created objects.
         * @param   url         The URL of the sink to create.
         * @return  A sink instance.
         */
        [[nodiscard]] virtual std::shared_ptr<Sink> Create(std::string const & url) = 0;

        /**
         * @brief   Returns a human readable id for the sink producer.
         * This id is also used to identify the producer within the factory.
         * @return  A description for the sink producer.
         */
        [[nodiscard]] virtual std::string GetId() const = 0;

        /**
         * @brief   Checks if this producer is capable to create the object.
         * @brief   url         The URL to match against.
         * @return  True, if this producer can create Sinks matching the given URL.
         */
        [[nodiscard]] virtual bool Match(std::string const & url) const = 0;
    };

    /**
     * @brief   Constructor.
     */
    SinkFactory() = delete;

    /**
     * @brief   Copy Constructor.
     */
    SinkFactory(SinkFactory const &) = delete;

    /**
     * @brief   Move Constructor.
     */
    SinkFactory(SinkFactory &&) = delete;

    /**
     * @brief   Destructor.
     */
    ~SinkFactory() = default;

    /**
     * @brief   Assignment.
     * @return  this.
     */
    SinkFactory & operator=(SinkFactory const &) = delete;

    /**
     * @brief   Move Assignment.
     * @return  this.
     */
    SinkFactory & operator=(SinkFactory &&) = delete;

    /**
     * @brief   Create an instance of a specific sink.
     *
     * This method MAY return the very same object with this particular URL
     * if one has been created in the past already. Therefore the caller may
     * not assume exclusive ownership.
     * @param   url         the url of the sink to create.
     * @return  A pointer pointing to an object (which may be nullptr in case of failure).
     */
    static std::shared_ptr<Sink> Create(std::string const & url);

    /**
     * @brief   Returns the list of known sink producers.
     * @return  The list of Ids of known sink producers.
     */
    static std::vector<std::string> GetProducerList();

    /**
     * @brief   Registers a producer, which can create Sink instances of a specific URL.
     * @param   producer    the Producer instance.
     */
    static void Register(std::unique_ptr<SinkFactory::Producer> && producer);

    /**
     * @brief   Registers all known default producers.
     */
    static void RegisterDefaultProducers();
};


}


#endif
