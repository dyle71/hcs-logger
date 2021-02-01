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
     * @brief   Time point when this registry came to live.
     */
    std::chrono::system_clock::time_point birth_;

    /**
     * @brief   All known loggers.
     */
    std::map<std::string, std::shared_ptr<headcode::logger::Logger>> loggers_;

    /**
     * @brief   Constructor.
     */
    Registry() : birth_{std::chrono::system_clock::now()} {
    }

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


/**
 * @brief   Returns the Registry singleton.
 * @return  The one and only registry instance.
 */
static Registry & GetRegistryInstance() {
    static Registry registry;
    return registry;
}


Logger::Logger(std::string name) : name_{std::move(name)} {
    ancestors_ = CreateListOfAncestors(name_);
}


std::chrono::system_clock::time_point Logger::GetBirth() {
    auto const & registry = GetRegistryInstance();
    return registry.birth_;
}


std::shared_ptr<Logger> Logger::GetLogger(std::string name) {

    name = FixLoggerName(name);
    auto & registry = GetRegistryInstance();

    auto lock_write = registry.LockWrite();
    auto iter = registry.loggers_.find(name);
    if (iter == registry.loggers_.end()) {
        registry.loggers_.emplace(name, std::shared_ptr<Logger>(new Logger{name}));
    }

    return registry.loggers_[name];
}


std::string Logger::GetName() const {
    if (name_.empty()) {
        return "<root>";
    }
    return name_;
}
