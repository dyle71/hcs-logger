/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger_core.hpp>

#include <headcode/logger/sink.hpp>
#include <headcode/logger/sink_factory.hpp>

#include <map>
#include <shared_mutex>
#include <utility>

using namespace headcode::logger;


namespace headcode::logger {


/**
 * @brief   Our logger registry.
 * The is the "database" of all known loggers.
 */
struct LoggerRegistry {
    /**
     * @brief   The registry singleton.
     */
    static LoggerRegistry registry_;

    /**
     * @brief   Prevent race conditions.
     */
    mutable std::shared_mutex mutex_;

    /**
     * @brief   Counter of created loggers so far.
     */
    unsigned int logger_count{0};

    /**
     * @brief   Time point when this registry came to live.
     */
    std::chrono::system_clock::time_point birth_;

    /**
     * @brief   All known loggers.
     */
    std::map<std::string, std::unique_ptr<headcode::logger::Logger>> loggers_;

    /**
     * @brief   Constructor.
     */
    LoggerRegistry() : birth_{std::chrono::system_clock::now()} {
    }

    /**
     * @brief   Copy Constructor.
     */
    LoggerRegistry(LoggerRegistry const &) = delete;

    /**
     * @brief   Move Constructor.
     */
    LoggerRegistry(LoggerRegistry &&) = delete;

    /**
     * @brief   Destructor
     */
    ~LoggerRegistry() = default;

    /**
     * @brief   Assignment
     * @return  this
     */
    LoggerRegistry & operator=(LoggerRegistry const &) = delete;

    /**
     * @brief   Move Assignment
     * @return  this
     */
    LoggerRegistry & operator=(LoggerRegistry &&) = delete;

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


/**
 * @brief   Singleton storage.
 */
LoggerRegistry LoggerRegistry::registry_;


}


/**
 * @brief   Creates the list of dot-separated names of all ancestors.
 *
 * E.g. given "foo.bar.baz" the list will be {"foo.bar", "foo", ""}.
 *
 * @param   name        name of the current logger.
 * @return  list of ancestor names.
 */
static std::list<std::string> CreateListOfAncestors(std::string name) {

    std::list<std::string> res;

    if (!name.empty()) {
        auto pos = name.rfind('.');
        while (pos != std::string::npos) {
            name = name.substr(0, pos);
            res.push_back(name);
            pos = name.rfind('.');
        }
        res.emplace_back(std::string{});
    }

    return res;
}


/**
 * @brief   Examine a logger name and make some corrections.
 * @param   name        the name provided by the user.
 * @return  The fixed name to be used inside here in the registry.
 */
std::string FixLoggerName(std::string name) {

    if (name.empty()) {
        return name;
    }
    if (name == "<root>") {
        return std::string{};
    }

    auto double_dots = name.find("..");
    while (double_dots != std::string::npos) {
        name.replace(double_dots, 2, ".");
        double_dots = name.find("..");
    }

    size_t left = 0;
    size_t right = name.size();
    while ((left < name.size()) && (name[left] == '.')) {
        ++left;
    }
    while ((right > left) && (name[right - 1] == '.')) {
        --right;
    }

    name = name.substr(left, right - left);

    return name;
}


#ifdef DEBUG
void LoggerRegistryPurge() {
    auto lock = LoggerRegistry::registry_.LockWrite();
    LoggerRegistry::registry_.loggers_.clear();
    LoggerRegistry::registry_.logger_count = 0;
}
#endif


Logger::Logger(std::string name, unsigned int id) : name_{std::move(name)}, id_(id) {
    ancestors_ = CreateListOfAncestors(name_);
}


void Logger::AddSink(std::shared_ptr<Sink> sink) {

    if (sink == nullptr) {
        return;
    }

    bool present = false;
    for (auto iter = sinks_.begin(); iter != sinks_.end() && !present; ++iter) {
        present = (*iter).lock().get() == sink.get();
    }
    if (present) {
        return;
    }

    sinks_.push_back(sink);
}


std::chrono::system_clock::time_point Logger::GetBirth() {
    auto lock = LoggerRegistry::registry_.LockRead();
    return LoggerRegistry::registry_.birth_;
}


Logger * Logger::GetLogger(std::string name) {

    name = FixLoggerName(name);

    auto lock_write = LoggerRegistry::registry_.LockWrite();

    if (LoggerRegistry::registry_.logger_count == 0) {

        // create root logger
        LoggerRegistry::registry_.loggers_.clear();

        auto logger = std::unique_ptr<Logger>(new Logger{std::string{}, LoggerRegistry::registry_.logger_count++});
        if (name.empty()) {
            logger->SetSink(SinkFactory::Create("stderr:"));
            logger->SetBarrier(Level::kWarning);
            LoggerRegistry::registry_.loggers_.emplace(name, std::move(logger));
        }
    }

    auto iter = LoggerRegistry::registry_.loggers_.find(name);
    if (iter == LoggerRegistry::registry_.loggers_.end()) {
        auto logger = std::unique_ptr<Logger>(new Logger{name, LoggerRegistry::registry_.logger_count++});
        logger->SetBarrier(Level::kUndefined);
        LoggerRegistry::registry_.loggers_.emplace(name, std::move(logger));
        iter = LoggerRegistry::registry_.loggers_.find(name);
    }

    return iter->second.get();
}


std::list<std::string> Logger::GetLoggers() {

    std::list<std::string> res;
    auto lock_read = LoggerRegistry::registry_.LockRead();
    for (auto const & [_, logger] : LoggerRegistry::registry_.loggers_) {
        res.push_back(logger->GetName());
    }

    return res;
}


std::string Logger::GetName() const {
    if (name_.empty()) {
        return "<root>";
    }
    return name_;
}


Logger * Logger::GetParentLogger() const {

    if (name_.empty()) {
        return nullptr;
    }

    auto lock_read = LoggerRegistry::registry_.LockRead();
    for (auto const & name : ancestors_) {
        auto iter = LoggerRegistry::registry_.loggers_.find(name);
        if (iter != LoggerRegistry::registry_.loggers_.end()) {
            return iter->second.get();
        }
    }

    return nullptr;
}


void Logger::Log(Event const & event) {

    ++events_logged_;

    if (GetBarrier() > 0) {
        Push(event);
    } else {
        auto parent = GetParentLogger();
        if (parent) {
            parent->Log(event);
        }
    }
}


void Logger::Push(Event const & event) {

    if (sinks_.empty()) {
        auto parent = GetParentLogger();
        if (parent) {
            parent->Push(event);
        }
    } else {
        for (auto & sink : sinks_) {
            auto real_sink = sink.lock();
            if (real_sink.get() != nullptr) {
                real_sink->Log(event);
            }
        }
    }
}


void Logger::SetBarrier(int barrier) {

    if (barrier < -1) {
        barrier = -1;
    }

    if (name_.empty() && (barrier < 0)) {
        // No kUndefined at the root logger.
        return;
    }

    barrier_ = barrier;
}


void Logger::SetBarrier(Level barrier) {
    SetBarrier(static_cast<int>(barrier));
}


void Logger::SetSink(std::shared_ptr<Sink> sink) {
    sinks_.clear();
    if (sink != nullptr) {
        sinks_.push_back(sink);
    }
}
