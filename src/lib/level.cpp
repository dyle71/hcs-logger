/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <array>

#include <headcode/logger/level.hpp>


/**
 * @brief   Caps the log level.
 * If the number is less than kUndefined, it will be set to kUndefined.
 * If the number is higher than kDebug, it sell be set to kDebug.
 * Else it is left unchanged.
 * @param   level       The log level.
 * @return  The log level capped.
 */
int CapLevel(int level) {

    if (level < static_cast<int>(headcode::logger::Level::kUndefined)) {
        level = static_cast<int>(headcode::logger::Level::kUndefined);
    }
    if (level > static_cast<int>(headcode::logger::Level::kDebug)) {
        level = static_cast<int>(headcode::logger::Level::kDebug);
    }

    return level;
}


/**
 * @brief   Returns a human readable text for a log level.
 * @param   level           The log level.
 * @return  A text describing the log level.
 */
std::string const & GetLevelText(int level) {
    static std::array<std::string, 6> const known_level_texts = {
            "undefined", "silent", "critical", "warning", "info", "debug"};
    return known_level_texts[CapLevel(level) + 1];
}


std::string const & headcode::logger::GetLevelText(headcode::logger::Level level) {
    return ::GetLevelText(static_cast<int>(level));
}
