/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/formatter.hpp>

#include <headcode/logger/event.hpp>
#include <headcode/logger/level.hpp>
#include <headcode/logger/logger_core.hpp>

#include <tuple>
#include <vector>

using namespace headcode::logger;


/**
 * @brief   Terminal Color Code used to reset color encoding.
 */
static std::string const kColorReset{"\x1B[0m"};


/**
 * @brief   Terminal Color Code used for critical events.
 */
static std::string const kColorCritical{"\x1B[1;38;5;9m"};


/**
 * @brief   Terminal Color Code used for warning events.
 */
static std::string const kColorWarning{"\x1B[1;38;5;11m"};


/**
 * @brief   Terminal Color Code used for info events.
 */
static std::string const kColorInfo{"\x1B[38;5;15m"};


/**
 * @brief   Terminal Color Code used for debug events.
 */
static std::string const kColorDebug{"\x1B[38;5;244m"};


/**
 * @brief   Gets the pre and post default terminal color strings for the levels.
 * @param   level           the log level.
 * @return  The pre and post default terminal color string.
 */
static std::tuple<std::string, std::string> GetDefaultColors(int level) {

    switch (level) {
        case static_cast<int>(Level::kCritical):
            return std::tuple<std::string, std::string>{kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return std::tuple<std::string, std::string>{kColorWarning, kColorReset};

        case static_cast<int>(Level::kInfo):
            return std::tuple<std::string, std::string>{kColorInfo, kColorReset};

        default:
            return std::tuple<std::string, std::string>{kColorDebug, kColorReset};
    }
}


/**
 * @brief   Gets the pre and post terminal color strings for the time of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetTimeStringColors(Event const & event) {
    return GetDefaultColors(event.GetLevel());
}


/**
 * @brief   Gets the pre and post terminal color strings for the level of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetLevelStringColors(Event const & event) {
    return GetDefaultColors(event.GetLevel());
}


/**
 * @brief   Gets the pre and post terminal color strings for the logger of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetLoggerStringColors(Event const & event) {

    static std::vector<std::string> color_loggers;
    if (color_loggers.empty()) {
        color_loggers.resize(232 - 16);
        for (int i = 16; i < 232; ++i) {
            color_loggers[i - 16] = std::string{"\x1B[38;5;"} + std::to_string(i) + "m";
        }
    }

    // The colors in the near neighbourhood are very much the same with a small epsilon.
    // Therefore, based on the continuous index number of loggers, we make jumps with
    // a prime number (hopefully acting as an algebraic generator), to get distinct colours
    // for loggers with consecutive IDs.

    size_t index = (event.GetLogger()->GetId() * 11) % color_loggers.size();
    switch (event.GetLevel()) {
        case static_cast<int>(Level::kCritical):
            return std::tuple<std::string, std::string>{kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return std::tuple<std::string, std::string>{kColorWarning, kColorReset};

        default:
            return std::tuple<std::string, std::string>{color_loggers[index], kColorReset};
    }
}


/**
 * @brief   Gets the pre and post terminal color strings for a line of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetLineStringColors(Event const & event) {
    return GetDefaultColors(event.GetLevel());
}


std::string ColorDarkBackgroundFormatter::Format_(Event const & event) {

    auto lines = SplitMessageIntoLines(event.str());
    auto time_string = CreateTimeString(event);
    auto level_string = CreateLevelString(event);
    auto logger_string = CreateLoggerString(event);

    auto [time_pre, time_post] = GetTimeStringColors(event);
    auto [level_pre, level_post] = GetLevelStringColors(event);
    auto [logger_pre, logger_post] = GetLoggerStringColors(event);
    auto [line_pre, line_post] = GetLineStringColors(event);

    std::stringstream ss;
    for (auto const & line : lines) {
        ss << time_pre << time_string << time_post << " " << level_pre << level_string << level_post
           << (logger_string.empty() ? "" : " ") << logger_pre << logger_string << logger_post << line_pre << ": "
           << line << line_post;
    }

    return ss.str();
}
