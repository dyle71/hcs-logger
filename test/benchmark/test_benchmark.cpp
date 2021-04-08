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
#include <iomanip>
#include <iostream>

#include <headcode/logger/logger.hpp>

#include "shared/ipsum_lorem.hpp"


static void NormalBig() {

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 10'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << kIpsumLoremText;
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto bps = (kIpsumLoremText.size() * 8 * loop_count) / static_cast<double>(milliseconds.count() / 1000.0);

    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout << "Benchmark 'normal_flow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec, "
              << std::setprecision(3) << bps << " bps." << std::endl;
}


static void NormalFlow() {

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'NormalFlow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
}


void NormalFlowFile() {

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kDebug);
    auto sink = headcode::logger::SinkFactory::Create("file:a.log");
    headcode::logger::Logger::GetLogger()->SetSink(sink);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'NormalFlowFile' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
}


void PrefetchFlowFile() {

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }
    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetBarrier(headcode::logger::Level::kDebug);
    auto sink = headcode::logger::SinkFactory::Create("file:a.log");
    logger->SetSink(sink);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{logger} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'PrefetchFlowFile' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
}


void PrefetchNormalFlow() {

    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetBarrier(headcode::logger::Level::kDebug);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{logger} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'PrefetchNormalFlow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
}


void SilentFlow() {

    headcode::logger::Logger::GetLogger()->SetBarrier(headcode::logger::Level::kSilent);

    auto start = std::chrono::system_clock::now();
    std::uint64_t loop_count = 100'000;

    for (std::uint64_t i = 0; i < loop_count; ++i) {
        headcode::logger::Debug{} << "Debug";
    }

    auto end = std::chrono::system_clock::now();

    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Benchmark 'SilentFlow' - " << loop_count << " Debug() in " << milliseconds.count() << " msec."
              << std::endl;
}


int main(int, char **) {

    NormalFlow();
    SilentFlow();
    PrefetchNormalFlow();
    NormalFlowFile();
    PrefetchFlowFile();
    NormalBig();

    return 0;
}
