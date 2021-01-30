/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger_core.hpp>

#include <map>
#include <shared_mutex>
#include <utility>

using namespace headcode::logger;

/**
 * @brief   Our logger registry.
 * The is the "database" of all known loggers.
 */
class Registry {
public:
    /**
     * @brief   Prevent race conditions.
     */
    mutable std::shared_mutex mutex_;

    /**
     * @brief   All known loggers.
     */
    std::map<std::string, std::shared_ptr<Logger>> loggers_;

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


/**
 * @brief   Returns the Registry singleton.
 * @return  The one and only registry instance.
 */
static Registry & GetRegistryInstance() {
    static Registry registry;
    return registry;
}


Logger::Logger(std::string name) : name_{std::move(name)} {
}


std::shared_ptr<Logger> Logger::GetLogger(std::string name) {

    std::shared_ptr<Logger> res;

    {
        auto & registry = GetRegistryInstance();
        auto lock_read = registry.LockWrite();
        auto iter = registry.loggers_.find(name);
        if (iter == registry.loggers_.end()) {
            registry.loggers_.emplace(name, std::shared_ptr<Logger>(new Logger{name}));
        }

        res = registry.loggers_[name];
    }

    return res;
}
