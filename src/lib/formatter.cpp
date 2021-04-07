/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/formatter.hpp>

#include <headcode/logger/event.hpp>
#include <headcode/logger/logger_core.hpp>

#include <ctime>

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

    auto const logger = event.GetLogger();
    if (logger == nullptr) {
        return {};
    }

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
