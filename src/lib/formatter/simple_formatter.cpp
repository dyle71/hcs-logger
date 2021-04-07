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


std::string SimpleFormatter::Format_(Event const & event) {
    return event.GetMessage();
}
