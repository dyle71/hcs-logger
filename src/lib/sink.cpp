/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/sink.hpp>

#include "sink/console_sink.hpp"
#include "sink/file_sink.hpp"
#include "sink/null_sink.hpp"
#include "sink/syslog_sink.hpp"

#include <headcode/logger/event.hpp>
#include <headcode/logger/formatter.hpp>

#include <headcode/url/url.hpp>

#include <map>
#include <utility>

using namespace headcode::logger;
using namespace headcode::url;


namespace headcode::logger {


/**
 * @brief   Our sink registry.
 * The is the "database" of all known sinks.
 */
struct Registry {
    /**
     * @brief   The registry singleton.
     */
    static Registry registry_;

    /**
     * @brief   Prevent race conditions.
     */
    mutable std::shared_mutex mutex_;

    /**
     * @brief   All known sinks.
     */
    std::map<std::string, std::unique_ptr<headcode::logger::Sink>> sinks_;

    /**
     * @brief   Constructor.
     */
    Registry() = default;

    /**
     * @brief   Copy Constructor.
     */
    Registry(Registry const &) = delete;

    /**
     * @brief   Move Constructor.
     */
    Registry(Registry &&) = delete;

    /**
     * @brief   Destructor
     */
    ~Registry() = default;

    /**
     * @brief   Assignment
     * @return  this
     */
    Registry & operator=(Registry const &) = delete;

    /**
     * @brief   Move Assignment
     * @return  this
     */
    Registry & operator=(Registry &&) = delete;

    /**
     * @brief   Get a Read-Only lock --> many can read, no-one can write.
     * @return  A lock which enables us to read states of this object in a thread-safe manner.
     */
    [[nodiscard]] std::shared_lock<std::shared_mutex> LockRead() const {
        return std::shared_lock<std::shared_mutex>(mutex_);
    }

    /**
     * @brief   Get a Read-Write exclusive lock --> we have exclusive access.
     * @return  A lock which enables us to read and write states of this object in a thread-safe manner.
     */
    [[nodiscard]] std::unique_lock<std::shared_mutex> LockWrite() {
        return std::unique_lock<std::shared_mutex>(mutex_);
    }
};


}


Sink::Sink(std::string url) : url_{std::move(url)}, formatter_{std::make_unique<StandardFormatter>()} {
}


Sink * Sink::GetSink(std::string url_string) {

    auto lock = Registry::registry_.LockWrite();

    auto url = URL{url_string}.Normalize();
    if (!url.IsValid()) {
        return nullptr;
    }

    auto iter = Registry::registry_.sinks_.find(url.GetURL());
    if (iter == Registry::registry_.sinks_.end()) {

        if (url.GetURL() == "stderr:") {
            Registry::registry_.sinks_.emplace(url.GetURL(),
                                               std::make_unique<headcode::logger::ConsoleSink>(url.GetURL()));
        } else if (url.GetURL() == "stdout:") {
            Registry::registry_.sinks_.emplace(url.GetURL(),
                                               std::make_unique<headcode::logger::ConsoleSink>(url.GetURL()));
        } else if (url.GetURL() == "syslog:") {
            Registry::registry_.sinks_.emplace("syslog", std::make_unique<headcode::logger::SyslogSink>());
        } else if (url.GetScheme() == "file") {
            Registry::registry_.sinks_.emplace(url.GetURL(),
                                               std::make_unique<headcode::logger::FileSink>(url.GetURL()));
        } else if (url.GetURL() == "null:") {
            Registry::registry_.sinks_.emplace(url.GetURL(), std::make_unique<headcode::logger::NullSink>());
        } else {
            return nullptr;
        }

        iter = Registry::registry_.sinks_.find(url.GetURL());
    }

    return iter->second.get();
}


std::vector<std::string> Sink::GetSinks() {

    auto lock = Registry::registry_.LockRead();

    std::vector<std::string> res;
    for (auto & p : Registry::registry_.sinks_) {
        res.push_back(p.first);
    }

    return res;
}


std::string Sink::Format(Event const & event) {
    return formatter_->Format(event);
}


std::string Sink::GetDescription() const {
    return GetDescription_();
}


void Sink::Log(Event const & event) {
    auto level = event.GetLevel();
    if ((level > 0) && (level <= GetBarrier())) {
        events_logged_++;
        Log_(event);
    }
}


void Sink::SetBarrier(int barrier) {
    if (barrier < 0) {
        return;
    }
    barrier_ = barrier;
}


void Sink::SetFormatter(std::unique_ptr<Formatter> && formatter) {
    if (formatter.get() != nullptr) {
        formatter_.reset();
        formatter_.swap(formatter);
    }
}


void Sink::SetBarrier(Level barrier) {
    SetBarrier(static_cast<int>(barrier));
}
