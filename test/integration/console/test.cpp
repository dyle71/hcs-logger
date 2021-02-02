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

    Logger::GetLogger()->SetBarrier(1000);
    Critical() << "This is a critical message.";
    Warning() << "This is a warning message.";
    Info() << "This is an info message.";
    Debug() << "This is a debug message.";

    Critical("foo") << "This is a critical message.";
    Warning("foo") << "This is a warning message.";
    Info("foo") << "This is an info message.";
    Debug("foo") << "This is a debug message.";

    Critical("foo.bar") << "This is a critical message.";
    Warning("foo.bar") << "This is a warning message.";
    Info("foo.bar") << "This is an info message.";
    Debug("foo.bar") << "This is a debug message.";

    return 0;
}
