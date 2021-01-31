/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */


#include <headcode/logger/event.hpp>
#include <headcode/logger/logger_core.hpp>

using namespace headcode::logger;


Event::Event(int level, std::string logger_name)
        : time_point_{std::chrono::system_clock::now()},
          logger_name_{std::move(logger_name)},
          level_{level},
          since_start_{std::chrono::duration_cast<std::chrono::microseconds>(time_point_ - Logger::GetBirth())} {
}
