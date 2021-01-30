/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.  
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_LOGGER_CORE_HPP
#define HEADCODE_SPACE_LOGGER_LOGGER_CORE_HPP

#include <memory>
#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   This is the main core Logger class.
 */
class Logger {

    std::string name_;  //!< @brief The name of this logger.

public:

    /**
     * @brief   Copy constructor
     */
    Logger(Logger const &) = delete;

    /**
     * @brief   Move constructor
     */
    Logger(Logger &&) = delete;

    /**
     * @brief   Destructor
     */
    virtual ~Logger() = default;

    /**
     * @brief   Assignment operator.
     */
    Logger & operator=(Logger const &) = delete;

    /**
     * @brief   Move operator.
     */
    Logger & operator=(Logger &&) = delete;

    /**
     * @brief   Get the Logger instance by name.
     * If a logger instance with the given name is not existing,
     * a new instance will be created.
     * @param   name        the name of the logger instance.
     * @return  The logger instance with that name.
     */
    static std::shared_ptr<Logger> GetLogger(std::string name);

    /**
     * @brief   Returns the name of this logger instance.
     * @return  The name of this logger.
     */
    [[nodiscard]] std::string GetName() const {
        return name_;
    }

private:

    /**
     * @brief   Logger constructor
     * @param   name            the name of the logger.
     */
    explicit Logger(std::string name);
};


}


#endif
