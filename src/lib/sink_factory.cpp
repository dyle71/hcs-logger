/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/sink_factory.hpp>

#include "sink/console_sink.hpp"
#include "sink/file_sink.hpp"
#include "sink/null_sink.hpp"
#include "sink/syslog_sink.hpp"

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>

using namespace headcode::logger;


namespace headcode::logger {


/**
 * @brief   Our sink producer registry.
 * The is the "database" of all known sink producers.
 */
struct SinkProducerRegistry {

    /**
     * @brief   The registry singleton.
     */
    static SinkProducerRegistry registry;

    /**
     * @brief   Prevent race conditions.
     */
    mutable std::shared_mutex mutex;

    /**
     * @brief   All known sink producers by Id.
     */
    std::map<std::string, std::unique_ptr<headcode::logger::SinkFactory::Producer>> sink_producers;

    /**
     * @brief   Constructor.
     */
    SinkProducerRegistry() = default;

    /**
     * @brief   Copy Constructor.
     */
    SinkProducerRegistry(SinkProducerRegistry const &) = delete;

    /**
     * @brief   Move Constructor.
     */
    SinkProducerRegistry(SinkProducerRegistry &&) = delete;

    /**
     * @brief   Destructor
     */
    ~SinkProducerRegistry() = default;

    /**
     * @brief   Assignment
     * @return  this
     */
    SinkProducerRegistry & operator=(SinkProducerRegistry const &) = delete;

    /**
     * @brief   Move Assignment
     * @return  this
     */
    SinkProducerRegistry & operator=(SinkProducerRegistry &&) = delete;

    /**
     * @brief   Get a Read-Only lock --> many can read, no-one can write.
     * @return  A lock which enables us to read states of this object in a thread-safe manner.
     */
    [[nodiscard]] std::shared_lock<std::shared_mutex> LockRead() const {
        return std::shared_lock<std::shared_mutex>(mutex);
    }

    /**
     * @brief   Get a Read-Write exclusive lock --> we have exclusive access.
     * @return  A lock which enables us to read and write states of this object in a thread-safe manner.
     */
    [[nodiscard]] std::unique_lock<std::shared_mutex> LockWrite() {
        return std::unique_lock<std::shared_mutex>(mutex);
    }
};


/**
 * @brief   Singleton storage.
 */
SinkProducerRegistry SinkProducerRegistry::registry;


}


std::shared_ptr<Sink> SinkFactory::Create(std::string const & url) {

    RegisterDefaultProducers();

    auto lock = SinkProducerRegistry::registry.LockRead();
    std::shared_ptr<Sink> res;
    for (auto & p : SinkProducerRegistry::registry.sink_producers) {
        if (p.second->Match(url)) {
            res = p.second->Create(url);
            return res;
        }
    }

    return res;
}


std::vector<std::string> SinkFactory::GetProducerList() {

    auto lock = SinkProducerRegistry::registry.LockRead();
    std::vector<std::string> res;
    for (auto const & p : SinkProducerRegistry::registry.sink_producers) {
        res.push_back(p.second->GetId());
    }
    return res;
}


void SinkFactory::Register(std::unique_ptr<SinkFactory::Producer> && producer) {

    if (!producer) {
        return;
    }
    auto lock = SinkProducerRegistry::registry.LockWrite();
    SinkProducerRegistry::registry.sink_producers[producer->GetId()] = std::move(producer);
}


void SinkFactory::RegisterDefaultProducers() {
    static std::atomic_flag registered = ATOMIC_FLAG_INIT;
    if (!registered.test_and_set()) {
        ConsoleSink::RegisterProducer();
        FileSink::RegisterProducer();
        NullSink::RegisterProducer();
        SyslogSink::RegisterProducer();
    }
}
