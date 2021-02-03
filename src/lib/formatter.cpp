/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <ctime>
#include <list>
#include <map>
#include <vector>

#include <headcode/logger/logger_core.hpp>
#include <headcode/logger/formatter.hpp>

using namespace headcode::logger;


std::string Formatter::CreateLevelString(Event const & event) {

    // users may issue any number beyond debug --> cap those to "debug".
    auto level = static_cast<Level>(std::min<int>(event.GetLevel(), static_cast<int>(Level::kDebug)));

    std::string res;
    res.resize(32);
    snprintf(res.data(), 32, "(%-8s)", GetLevelText(level).data());
    res.resize(res.find_last_of(')') + 1);

    return res;
}


std::string Formatter::CreateLoggerString(Event const & event) {
    auto const & logger = event.GetLogger();
    if (logger->IsRootLogger()) {
        return std::string{};
    }
    return std::string{"{"} + logger->GetName() + "}";
}


std::string Formatter::CreateTimeString(Event const & event) {

    std::string res;

    std::time_t tt = std::chrono::system_clock::to_time_t(event.GetTimePoint());
    auto duration_since_epoch = event.GetTimePoint().time_since_epoch();
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
    auto ms_fraction = static_cast<int>(ms_since_epoch.count() - tt * 1000);
    struct tm tm {};
    gmtime_r(&tt, &tm);

    // ISO8601 oriented
    res.resize(std::string{"[YYYY-MM-DDThh:mm:ss,sss+00:00]"}.size() + 1);
    snprintf(res.data(),
             res.size(),
             "[%04d-%02d-%02dT%02d:%02d:%02d,%03d+00:00]",
             tm.tm_year + 1900,
             tm.tm_mon + 1,
             tm.tm_mday,
             tm.tm_hour,
             tm.tm_min,
             tm.tm_sec,
             ms_fraction);

    // cut the last 0
    res.resize(res.size() - 1);

    return res;
}


std::list<std::string> Formatter::SplitMessageIntoLines(std::string const & message) {

    std::list<std::string> res;
    if (message.empty()) {
        res.emplace_back("\n");
        return res;
    }

    size_t start = 0;
    size_t lf = 0;
    size_t end = message.size();
    while (start < end) {
        lf = message.find_first_of('\n', start);
        if (lf == std::string::npos) {
            res.push_back(message.substr(start, end - start) + "\n");
            start = end;
        } else {
            res.push_back(message.substr(start, lf - start + 1));
            start = lf + 1;
        }
    }

    return res;
}


std::string StandardFormatter::Format_(Event const & event) {

    auto lines = SplitMessageIntoLines(event.str());
    auto time_string = CreateTimeString(event);
    auto level_string = CreateLevelString(event);
    auto logger_string = CreateLoggerString(event);

    std::stringstream ss;
    for (auto const & line : lines) {
        ss << time_string << " " << level_string << (logger_string.empty() ? "" : " ") << logger_string << ": " << line;
    }

    return ss.str();
}


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
 * @brief   Gets the pre and post terminal color strings for the time of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetTimeStringColors(Event const & event) {

    switch (event.GetLevel()) {
        case static_cast<int>(Level::kCritical):
            return {kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return {kColorWarning, kColorReset};

        case static_cast<int>(Level::kInfo):
            return {kColorInfo, kColorReset};

        default:
            return {kColorDebug, kColorReset};
    }
}


/**
 * @brief   Gets the pre and post terminal color strings for the level of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetLevelStringColors(Event const & event) {

    switch (event.GetLevel()) {
        case static_cast<int>(Level::kCritical):
            return {kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return {kColorWarning, kColorReset};

        case static_cast<int>(Level::kInfo):
            return {kColorInfo, kColorReset};

        default:
            return {kColorDebug, kColorReset};
    }
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
            return {kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return {kColorWarning, kColorReset};

        default:
            return {color_loggers[index], kColorReset};
    }
}


/**
 * @brief   Gets the pre and post terminal color strings for a line of this event.
 * @param   event       the event.
 * @return  The pre and post terminal color string.
 */
static std::tuple<std::string, std::string> GetLineStringColors(Event const & event) {

    switch (event.GetLevel()) {
        case static_cast<int>(Level::kCritical):
            return {kColorCritical, kColorReset};

        case static_cast<int>(Level::kWarning):
            return {kColorWarning, kColorReset};

        case static_cast<int>(Level::kInfo):
            return {kColorInfo, kColorReset};

        default:
            return {kColorDebug, kColorReset};
    }
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
