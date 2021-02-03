/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_LEVEL_HPP
#define HEADCODE_SPACE_LOGGER_LEVEL_HPP

#include <string>

namespace headcode::logger {


/**
 * @brief   The different log levels.
 *
 * Every Log Event has a certain level. Loggers will pass on events to sink if (and only if) the
 * log barrier is *higher or equal to* the level in the event.
 *
 * E.g.     logger->GetBarrier() ==> kWarning
 *
 *          - kCritical ==> Forward
 *          - kWarning  ==> Forward
 *          - kInfo     ==> Drop
 *          - kDebug    ==> Drop
 *
 * To enable ALL messages set a log level as high as possible. Log Events may use any higher
 * number as kDebug, as the user wishes. The framework is confident with these listened, but
 * you are free to chip in any positive number of your liking (e.g. Trace = 100,
 * SuperTrace = 1000, ...)
 *
 * To enforce strict silence set the log level to kSilent.
 *
 * To use the log level of the parent logger instance set the log level to kUndefined.
 * This will then defer the event guarding to the parent Logger instance.
 */
enum class Level : int {

    kUndefined = -1,        //!< @brief Undefined log level --> check log level of parent logger instance.
    kSilent = 0,            //!< @brief This level drops **all** (incl. critical one) log events: "Shush!"
    kCritical = 1,          //!< @brief Critical event: emergency! Any processing beyond this point is undefined,
                            //!< potential loss of data: "Panic!".
    kWarning = 2,        //!< @brief Warning! At the moment all is still fine, but computations later on could lead into
                         //!< a more problematic state: "Behave! There are problems ahead, I'm telling you!".
    kInfo = 3,           //!< @brief Mere regular information to an user: "I've done that, here you are.".
    kDebug = 4           //!< @brief Reveals in deep sophisticated information for developers.
};


/**
 * @brief   Returns a human readable text for a log level.
 * @param   level           The log level.
 * @return  A text describing the log level.
 */
std::string const & GetLevelText(Level level);


}


#endif
