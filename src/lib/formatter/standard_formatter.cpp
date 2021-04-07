/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/formatter.hpp>

#include <headcode/logger/event.hpp>

using namespace headcode::logger;


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
