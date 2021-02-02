/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <fstream>
#include <filesystem>
#include <regex>

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

    auto log_file = std::filesystem::path{"test.log"};
    if (std::filesystem::exists(log_file)) {
        std::filesystem::remove(log_file);
    }

    {
        auto sink = std::make_shared<headcode::logger::FileSink>("test.log");
        sink->SetFormatter(std::make_shared<headcode::logger::SimpleFormatter>());

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


TEST(Sink, description) {

    auto null_sink = headcode::logger::NullSink{};
    ASSERT_FALSE(null_sink.GetDescription().empty());
    EXPECT_STREQ(null_sink.GetDescription().c_str(), "NullSink");

    auto file_sink = headcode::logger::FileSink{};
    ASSERT_FALSE(file_sink.GetDescription().empty());
    EXPECT_STREQ(file_sink.GetDescription().c_str(), "FileSink to a.log");

    auto console_sink = headcode::logger::ConsoleSink{};
    ASSERT_FALSE(console_sink.GetDescription().empty());
    EXPECT_STREQ(console_sink.GetDescription().c_str(), "ConsoleSink");
}


TEST(Sink, fomatters) {

    auto a_sink = std::make_shared<headcode::logger::FileSink>("a.log");

    EXPECT_NE(a_sink->GetFormatter(), nullptr);
    a_sink->SetFormatter(nullptr);
    EXPECT_NE(a_sink->GetFormatter(), nullptr);
    auto a_formatter = std::make_shared<headcode::logger::StandardFormatter>();
    a_sink->SetFormatter(a_formatter);
    EXPECT_EQ(a_sink->GetFormatter(), a_formatter);
}


TEST(Sink, multiple) {

    for (auto const & filename : {"a.log", "b.log", "c.log", "d.log", "e.log", "f.log"}) {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }

    auto a_sink = std::make_shared<headcode::logger::FileSink>("a.log");
    auto b_sink = std::make_shared<headcode::logger::FileSink>("b.log");
    auto c_sink = std::make_shared<headcode::logger::FileSink>("c.log");
    auto d_sink = std::make_shared<headcode::logger::FileSink>("d.log");
    auto e_sink = std::make_shared<headcode::logger::FileSink>("e.log");

    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetSink(a_sink);
    logger->AddSink(b_sink);
    logger->AddSink(c_sink);
    logger->AddSink(d_sink);
    logger->AddSink(e_sink);

    a_sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());
    b_sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());
    c_sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());
    d_sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());
    e_sink->SetFormatter(std::make_shared<headcode::logger::StandardFormatter>());

    logger->SetBarrier(headcode::logger::Level::kDebug);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));

    a_sink->SetBarrier(headcode::logger::Level::kDebug);
    EXPECT_EQ(a_sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));
    b_sink->SetBarrier(headcode::logger::Level::kInfo);
    EXPECT_EQ(b_sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kInfo));
    c_sink->SetBarrier(headcode::logger::Level::kWarning);
    EXPECT_EQ(c_sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));
    d_sink->SetBarrier(headcode::logger::Level::kCritical);
    EXPECT_EQ(d_sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kCritical));
    e_sink->SetBarrier(headcode::logger::Level::kSilent);
    EXPECT_EQ(e_sink->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    headcode::logger::Debug() << "This is a debug message.";
    headcode::logger::Info() << "This is an info message.";
    headcode::logger::Warning() << "This is a warning message.";
    headcode::logger::Critical() << "This is a critical message.";
    headcode::logger::Event(headcode::logger::Level::kSilent) << "This is a silent message.";

    ASSERT_TRUE(std::filesystem::exists("a.log"));
    ASSERT_TRUE(std::filesystem::exists("b.log"));
    ASSERT_TRUE(std::filesystem::exists("c.log"));
    ASSERT_TRUE(std::filesystem::exists("d.log"));
    EXPECT_FALSE(std::filesystem::exists("e.log"));

    auto CollectEvents = [&](std::string const & name, std::list<std::string> & l) {
        static std::regex const re{R"(\[\d\d\d\d-\d\d-\d\dT\d\d\:\d\d\:\d\d,\d\d\d\+\d\d\:\d\d\] \((\S*) *\)\: .*)"};
        std::smatch match;
        std::ifstream file;
        file.open(name, std::ios::in);
        do {
            std::string line;
            std::getline(file, line);
            if (line.empty()) {
                continue;
            }
            std::regex_match(line, match, re);
            ASSERT_EQ(match.size(), 2u);
            l.push_back(match[1]);
        } while (file.good());
        file.close();
    };

    std::list<std::string> a_events;
    CollectEvents("a.log", a_events);
    std::list<std::string> a_expected = {"debug", "info", "warning", "critical"};
    EXPECT_EQ(a_events, a_expected);

    std::list<std::string> b_events;
    CollectEvents("b.log", b_events);
    std::list<std::string> b_expected = {"info", "warning", "critical"};
    EXPECT_EQ(b_events, b_expected);

    std::list<std::string> c_events;
    CollectEvents("c.log", c_events);
    std::list<std::string> c_expected = {"warning", "critical"};
    EXPECT_EQ(c_events, c_expected);

    std::list<std::string> d_events;
    CollectEvents("d.log", d_events);
    std::list<std::string> d_expected = {"critical"};
    EXPECT_EQ(d_events, d_expected);
}
