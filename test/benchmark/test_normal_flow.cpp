/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <iostream>

#include <headcode/logger/logger.hpp>

#ifdef DEBUG

// Since we are operating on the very same logger subsystem, we need fresh instances in the tests.
extern void LoggerRegistryPurge();


TEST(Benchmark, normal_flow) {

    LoggerRegistryPurge();

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'normal_flow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
    EXPECT_GT(milliseconds.count(), 0u);
}


TEST(Benchmark, silent_flow) {

    LoggerRegistryPurge();

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kSilent);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'silent_flow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
    EXPECT_GT(milliseconds.count(), 0u);
}


TEST(Benchmark, prefetch_normal_flow) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetBarrier(headcode::logger::Level::kDebug);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{logger} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'prefetch_normal_flow' - " << loop_count << " Debug() in " << milliseconds.count()
              << " msec." << std::endl;
    EXPECT_GT(milliseconds.count(), 0u);
}


TEST(Benchmark, normal_flow_file) {

    LoggerRegistryPurge();

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }
    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);
    headcode::logger::Logger::GetLogger()->SetSink(std::make_shared<headcode::logger::FileSink>("a.log"));

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'normal_flow_file' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
    EXPECT_GT(milliseconds.count(), 0u);
}


TEST(Benchmark, prefetch_flow_file) {

    LoggerRegistryPurge();

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }
    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetBarrier(headcode::logger::Level::kDebug);
    logger->SetSink(std::make_shared<headcode::logger::FileSink>("a.log"));

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{logger} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'prefetch_flow_file' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
    EXPECT_GT(milliseconds.count(), 0u);
}


#endif
