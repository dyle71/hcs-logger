/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <fstream>

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Sink, barrier) {

    auto sink = std::make_shared<headcode::logger::NullSink>();

    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));
    sink->SetBarrier(headcode::logger::Level::kDebug);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));

    sink->SetBarrier(headcode::logger::Level::kInfo);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kInfo));

    sink->SetBarrier(headcode::logger::Level::kWarning);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));

    sink->SetBarrier(headcode::logger::Level::kCritical);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kCritical));

    sink->SetBarrier(headcode::logger::Level::kSilent);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    sink->SetBarrier(headcode::logger::Level::kUndefined);
    EXPECT_EQ(sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    sink->SetBarrier(1000);
    EXPECT_EQ(sink->GetBarrier(), 1000);

    sink->SetBarrier(-1000);
    EXPECT_EQ(sink->GetBarrier(), 1000);
}


TEST(Sink, file) {

    std::ofstream log_out;
    log_out.open("test.log", std::ios::out | std::ios::trunc);

    {
        auto sink = std::make_shared<headcode::logger::StreamSink>(log_out);

        auto event_debug = headcode::logger::Debug();
        event_debug << "This is a debug message." << std::endl;
        sink->Log(event_debug);

        auto event_info = headcode::logger::Info();
        event_info << "This is a info message." << std::endl;
        sink->Log(event_info);

        auto event_warning = headcode::logger::Warning();
        event_warning << "This is a warning message." << std::endl;
        sink->Log(event_warning);

        auto event_critical = headcode::logger::Critical();
        event_critical << "This is a critical message." << std::endl;
        sink->Log(event_critical);
    }

    log_out.close();

    std::ifstream log_in;
    log_in.open("test.log", std::ios::in);

    std::string line;
    std::getline(log_in, line);
    EXPECT_STREQ(line.c_str(), "This is a debug message.");
    std::getline(log_in, line);
    EXPECT_STREQ(line.c_str(), "This is a info message.");
    std::getline(log_in, line);
    EXPECT_STREQ(line.c_str(), "This is a warning message.");
    std::getline(log_in, line);
    EXPECT_STREQ(line.c_str(), "This is a critical message.");

    log_in.close();
}
