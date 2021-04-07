/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_SINK_MUTEX_SINK_HPP
#define HEADCODE_SPACE_LOGGER_SINK_MUTEX_SINK_HPP

#include <headcode/logger/sink.hpp>

#include <mutex>
#include <shared_mutex>
#include <string>


/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {


/**
 * @brief   A sink which safeguards concurrent logging actions.
 * This sink serializes parallel writes.
 */
class MutexSink : public Sink {

    std::mutex mutex_;        //!< @brief mutex to write to file.

protected:
    /**
     * @brief   Constructor
     * @param   url         the url by which this sink will be identified.
     */
    explicit MutexSink(std::string url) : Sink{url} {
    }

    /**
     * @brief   Gets a exclusive write lock to the sink.
     * @return  A lock object holding the write access privileges.
     */
    [[nodiscard]] std::unique_lock<std::mutex> LockWrite() {
        return std::unique_lock<std::mutex>{mutex_};
    }
};


}


#endif
