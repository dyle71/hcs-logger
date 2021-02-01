/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger.hpp>

using namespace headcode::logger;


int main(int , char **) {
    Logger::GetLogger()->SetBarrier(Level::kDebug);
    Debug() << "Hello World!";
    return 0;
}
