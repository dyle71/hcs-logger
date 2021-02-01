/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <fstream>
#include <memory>

#include <headcode/logger/logger.hpp>

using namespace headcode::logger;


void SetupLogging() {
    auto file_sink = std::make_shared<FileSink>();
    Logger::GetLogger()->SetSink(file_sink);
    Logger::GetLogger()->SetBarrier(Level::kDebug);
}


int main(int , char **) {

    SetupLogging();

    Debug() << "This is a debug message." << std::endl;
    Info() << "... and this is a warning message." << std::endl;
    Warning() << "See yourself been warned." << std::endl;
    Critical() << "Let's panic!" << std::endl;

    return 0;
}
