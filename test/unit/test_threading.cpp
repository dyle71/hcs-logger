/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <fstream>
#include <filesystem>
#include <functional>
#include <thread>

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Threading, concurrent) {

    if (std::filesystem::exists("thread.log")) {
        std::filesystem::remove("thread.log");
    }

    auto logger = headcode::logger::Logger::GetLogger();
    ASSERT_TRUE(logger != nullptr);
    auto sink = std::make_shared<headcode::logger::FileSink>("thread.log");
    sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());
    logger->SetSink(sink);
    logger->SetBarrier(headcode::logger::Level::kDebug);

    std::string log_message{"Log message of a thread."};
    std::uint64_t thread_count = 100;
    std::uint64_t loop_count = 1000;
    std::function<void()> thread_function = [&]() {
        for (std::uint64_t i = 0; i < loop_count; ++i) {
            headcode::logger::Debug{} << log_message;
        }
    };

    std::vector<std::thread> threads{thread_count};
    for (auto & thread : threads) {
        thread = std::thread{thread_function};
    }
    for (auto & thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    ASSERT_TRUE(std::filesystem::exists("thread.log"));
    std::ifstream log_file;
    log_file.open("thread.log", std::ios::in);
    std::uint64_t line_count = 0;
    while (log_file.good()) {
        std::string line;
        std::getline(log_file, line);
        if (!line.empty()) {
            line_count++;
            line = line.substr(line.size() - std::min(log_message.size(), line.size()), log_message.size());
            EXPECT_STREQ(line.c_str(), log_message.c_str());
        }
    }
    log_file.close();
    EXPECT_EQ(line_count, thread_count * loop_count);
}