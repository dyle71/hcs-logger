/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */


#include <headcode/logger/event.hpp>

#include <utility>

#include <headcode/logger/logger_core.hpp>

using namespace headcode::logger;


Event::Event(int level, std::string logger_name) : Event{level, Logger::GetLogger(std::move(logger_name))} {
}


Event::Event(Level level, std::string logger_name)
        : Event{static_cast<int>(level), Logger::GetLogger(std::move(logger_name))} {
}


Event::Event(Level level, std::shared_ptr<Logger> logger) : Event{static_cast<int>(level), std::move(logger)} {
}


Event::Event(int level, std::shared_ptr<Logger> logger)
        : time_point_{std::chrono::system_clock::now()},
          logger_{std::move(logger)},
          level_{level},
          since_start_{std::chrono::duration_cast<std::chrono::microseconds>(time_point_ - Logger::GetBirth())} {
}
