/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#ifndef HEADCODE_SPACE_LOGGER_VERSION_HPP
#define HEADCODE_SPACE_LOGGER_VERSION_HPP

#include <cstdint>
#include <string>


#ifndef MAKE_VERSION
#define MAKE_VERSION(x, y, z) ((x) << 24 | (y) << 16 | (z))
#endif


/**
 * @brief   Version check for 1.0.0
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSLoggerVersion_1_0_0() {
    return MAKE_VERSION(1, 0, 0);
}

/**
 * @brief   Version check for 1.0.1
 * @return  A value, representing the version.
 */
inline std::uint32_t GetHCSLoggerVersion_1_0_1() {
    return MAKE_VERSION(1, 0, 1);
}

/**
 * @brief   The headcode logger namespace
 */
namespace headcode::logger {

/**
 * @brief   Returns the current version of the headcode-logger.
 * @return  A value, representing the current version.
 */
inline std::uint32_t GetCurrentVersion() {
    return GetHCSLoggerVersion_1_0_1();
}

/**
 * @brief   Returns a version as string
 * @return  a string holding the current version.
 */
inline std::string GetVersionString() {
    return "1.0.1";
}


}


#endif
