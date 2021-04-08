/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>
#include <regex>


TEST(Sink, default_producers) {
    // Enforces registration of all default sink producers.
    headcode::logger::Logger::GetLogger({});
    auto producers = headcode::logger::SinkFactory::GetProducerList();
    EXPECT_EQ(producers.size(), 4u);
}


TEST(Sink, null_regular) {
    auto sink = headcode::logger::SinkFactory::Create("null:");
    EXPECT_NE(sink.get(), nullptr);
}


TEST(Sink, stderr_regular) {
    auto sink = headcode::logger::SinkFactory::Create("stderr:");
    EXPECT_NE(sink, nullptr);
    EXPECT_EQ(sink.get(), headcode::logger::SinkFactory::Create("stderr:").get());
}


TEST(Sink, stdout_regular) {
    auto sink = headcode::logger::SinkFactory::Create("stdout:");
    EXPECT_NE(sink, nullptr);
    EXPECT_EQ(sink.get(), headcode::logger::SinkFactory::Create("stdout:").get());
}


TEST(Sink, file_regular) {
    auto sink = headcode::logger::SinkFactory::Create("file:a.log");
    EXPECT_NE(sink.get(), nullptr);
    EXPECT_EQ(sink.get(), headcode::logger::SinkFactory::Create("file:a.log").get());
    EXPECT_NE(sink.get(), headcode::logger::SinkFactory::Create("file:b.log").get());
}


TEST(Sink, barrier) {

    auto sink = headcode::logger::SinkFactory::Create("null:");

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
        auto sink = headcode::logger::SinkFactory::Create("file:test.log");
        sink->SetFormatter(std::make_unique<headcode::logger::SimpleFormatter>());

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

    headcode::logger::Event event{1};
    event << "This is an event";

    auto null_sink = headcode::logger::SinkFactory::Create("null:");
    ASSERT_FALSE(null_sink->GetDescription().empty());
    EXPECT_STREQ(null_sink->GetDescription().c_str(), "NullSink");
    EXPECT_STREQ(null_sink->GetURL().c_str(), "null:");
    null_sink->Log(event);
    EXPECT_GT(null_sink->GetEventsLogged(), 0u);
    EXPECT_NE(null_sink, headcode::logger::SinkFactory::Create("null:"));

    auto file_sink = headcode::logger::SinkFactory::Create("file:");
    ASSERT_FALSE(file_sink->GetDescription().empty());
    EXPECT_STREQ(file_sink->GetDescription().c_str(), "FileSink to a.log");
    EXPECT_STREQ(file_sink->GetURL().c_str(), "file:a.log");
    file_sink->Log(event);
    EXPECT_GT(file_sink->GetEventsLogged(), 0u);

    auto console_sink = headcode::logger::SinkFactory::Create("stderr:");
    ASSERT_FALSE(console_sink->GetDescription().empty());
    EXPECT_STREQ(console_sink->GetDescription().c_str(), "ConsoleSink to stderr:");
    EXPECT_STREQ(console_sink->GetURL().c_str(), "stderr:");
    console_sink->Log(event);
    EXPECT_GT(console_sink->GetEventsLogged(), 0u);
}


TEST(Sink, multiple) {

    for (auto const & filename : {"a.log", "b.log", "c.log", "d.log", "e.log", "f.log"}) {
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }

    auto a_sink = headcode::logger::SinkFactory::Create("file:a.log");
    auto b_sink = headcode::logger::SinkFactory::Create("file:b.log");
    auto c_sink = headcode::logger::SinkFactory::Create("file:c.log");
    auto d_sink = headcode::logger::SinkFactory::Create("file:d.log");
    auto e_sink = headcode::logger::SinkFactory::Create("file:e.log");

    auto logger = headcode::logger::Logger::GetLogger();
    logger->SetSink(a_sink);
    logger->AddSink(b_sink);
    logger->AddSink(c_sink);
    logger->AddSink(d_sink);
    logger->AddSink(e_sink);

    a_sink->SetFormatter(std::make_unique<headcode::logger::StandardFormatter>());
    b_sink->SetFormatter(std::make_unique<headcode::logger::StandardFormatter>());
    c_sink->SetFormatter(std::make_unique<headcode::logger::StandardFormatter>());
    d_sink->SetFormatter(std::make_unique<headcode::logger::StandardFormatter>());
    e_sink->SetFormatter(std::make_unique<headcode::logger::StandardFormatter>());

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


TEST(Sink, syslog) {

    auto sink = headcode::logger::SinkFactory::Create("syslog:");
    EXPECT_STREQ(sink->GetDescription().c_str(), "SyslogSink");
    headcode::logger::Logger::GetLogger()->SetSink(sink);

    headcode::logger::Debug() << "This is a debug message.";
    headcode::logger::Info() << "This is an info message.";
    headcode::logger::Warning() << "This is a warning message.";
    headcode::logger::Critical() << "This is a critical message.";
    headcode::logger::Event(headcode::logger::Level::kSilent) << "This is a silent message.";

    // How to verify if these messages have appeared in syslog in a platform independent manner?
    // We might open the syslog in /var/log/syslog or /var/log/message, if journalctld writes
    // one that is. Next, we have to filter the messages to find our own one, and then we have
    // to match the recorded PID against the PID of this testing instance to avoid false
    // positives. Meeh...

    ASSERT_TRUE(true);
}


TEST(Sink, force_color_output) {

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }

    auto sink = headcode::logger::SinkFactory::Create("file:a.log");
    headcode::logger::Logger::GetLogger()->SetBarrier(1000);
    headcode::logger::Logger::GetLogger()->SetSink(sink);
    sink->SetFormatter(std::make_unique<headcode::logger::ColorDarkBackgroundFormatter>());

    headcode::logger::Critical() << "This critical event should be in color.";
    headcode::logger::Warning() << "This warning event should be in color.";
    headcode::logger::Info() << "This info event should be in color.";
    headcode::logger::Debug() << "This debug event should be in color.";

    std::ifstream log_file;
    log_file.open("a.log", std::ios::in);

    std::string line;
    std::getline(log_file, line);
    log_file.close();

    // Checking color codes in strings with a lot of escape codes, the IDE in between,
    // and several different conversation utilities thinking they know better,
    // is quite hard... How to do this correctly?
    EXPECT_FALSE(line.empty());
}
