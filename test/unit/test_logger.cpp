/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>


TEST(Logger, empty) {
    auto logger = headcode::logger::Logger::GetLogger();
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");
    EXPECT_EQ(logger->GetId(), 0u);
}

#ifdef DEBUG

// Since we are operating on the very same logger subsystem, we need fresh instances in the tests.
extern void LoggerRegistryPurge();


TEST(Logger, regular) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");

    auto logger_root = headcode::logger::Logger::GetLogger("<root>");
    ASSERT_TRUE(logger_root != nullptr);
    EXPECT_STREQ(logger_root->GetName().c_str(), "<root>");
    EXPECT_EQ(logger_root, logger);

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");

    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kUndefined));
}


TEST(Logger, list_loggers) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");

    auto logger_bar = headcode::logger::Logger::GetLogger("bar");
    ASSERT_TRUE(logger_bar != nullptr);
    EXPECT_STREQ(logger_bar->GetName().c_str(), "bar");

    auto logger_bar_baz = headcode::logger::Logger::GetLogger("bar.baz");
    ASSERT_TRUE(logger_bar_baz != nullptr);
    EXPECT_STREQ(logger_bar_baz->GetName().c_str(), "bar.baz");

    std::list<std::string> expected = {"<root>", "bar", "bar.baz", "foo"};
    EXPECT_EQ(headcode::logger::Logger::GetLoggers(), expected);
}


TEST(Logger, sinks) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");

    logger->AddSink(headcode::logger::SinkFactory::Create("null:"));
    EXPECT_EQ(logger->GetSinks().size(), 2u);
    logger->AddSink(headcode::logger::SinkFactory::Create("null:"));
    EXPECT_EQ(logger->GetSinks().size(), 3u);

    logger->SetSink(headcode::logger::SinkFactory::Create("null:"));
    EXPECT_EQ(logger->GetSinks().size(), 1u);

    logger->AddSink(headcode::logger::SinkFactory::Create("null:"));
    EXPECT_EQ(logger->GetSinks().size(), 2u);
}


TEST(Logger, parenting) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");
    EXPECT_EQ(logger->GetParentLogger(), nullptr);

    auto logger_foo_bar_baz = headcode::logger::Logger::GetLogger("foo.bar.baz");
    ASSERT_TRUE(logger_foo_bar_baz != nullptr);
    EXPECT_STREQ(logger_foo_bar_baz->GetName().c_str(), "foo.bar.baz");

    EXPECT_EQ(logger, logger_foo_bar_baz->GetParentLogger());

    auto logger_foo_bar = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar != nullptr);
    EXPECT_STREQ(logger_foo_bar->GetName().c_str(), "foo.bar");

    EXPECT_EQ(logger, logger_foo_bar->GetParentLogger());
    EXPECT_EQ(logger_foo_bar, logger_foo_bar_baz->GetParentLogger());

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");

    EXPECT_EQ(logger, logger_foo->GetParentLogger());
    EXPECT_EQ(logger_foo, logger_foo_bar->GetParentLogger());
    EXPECT_EQ(logger_foo_bar, logger_foo_bar_baz->GetParentLogger());
}


TEST(Logger, barrier_root) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");

    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));

    logger->SetBarrier(headcode::logger::Level::kDebug);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));

    logger->SetBarrier(headcode::logger::Level::kInfo);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kInfo));

    logger->SetBarrier(headcode::logger::Level::kWarning);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));

    logger->SetBarrier(headcode::logger::Level::kCritical);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kCritical));

    logger->SetBarrier(headcode::logger::Level::kSilent);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    logger->SetBarrier(headcode::logger::Level::kUndefined);
    EXPECT_EQ(logger->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    logger->SetBarrier(1000);
    EXPECT_EQ(logger->GetBarrier(), 1000);

    logger->SetBarrier(-1000);
    EXPECT_EQ(logger->GetBarrier(), 1000);
}


TEST(Logger, barrier_child) {

    LoggerRegistryPurge();

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");

    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kUndefined));

    logger_foo->SetBarrier(headcode::logger::Level::kDebug);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kDebug));

    logger_foo->SetBarrier(headcode::logger::Level::kInfo);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kInfo));

    logger_foo->SetBarrier(headcode::logger::Level::kWarning);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kWarning));

    logger_foo->SetBarrier(headcode::logger::Level::kCritical);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kCritical));

    logger_foo->SetBarrier(headcode::logger::Level::kSilent);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kSilent));

    logger_foo->SetBarrier(headcode::logger::Level::kUndefined);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kUndefined));

    logger_foo->SetBarrier(1000);
    EXPECT_EQ(logger_foo->GetBarrier(), 1000);

    logger_foo->SetBarrier(-1000);
    EXPECT_EQ(logger_foo->GetBarrier(), static_cast<int>(headcode::logger::Level::kUndefined));
}


TEST(Logger, naming) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);

    EXPECT_EQ(headcode::logger::Logger::GetLogger("."), logger);
    EXPECT_EQ(headcode::logger::Logger::GetLogger(".."), logger);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..."), logger);
    EXPECT_EQ(headcode::logger::Logger::GetLogger(".........."), logger);

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");

    EXPECT_EQ(headcode::logger::Logger::GetLogger(".foo"), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..foo"), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("...foo"), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo."), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo.."), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo..."), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger(".foo."), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..foo.."), logger_foo);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("...foo..."), logger_foo);

    auto logger_foo_bar = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar != nullptr);
    EXPECT_STREQ(logger_foo_bar->GetName().c_str(), "foo.bar");

    EXPECT_EQ(headcode::logger::Logger::GetLogger(".foo.bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..foo.bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("...foo.bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo.bar."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo.bar.."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo.bar..."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger(".foo.bar."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..foo.bar.."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("...foo.bar..."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo..bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo...bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("foo.....bar"), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger(".foo..bar."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("..foo..bar.."), logger_foo_bar);
    EXPECT_EQ(headcode::logger::Logger::GetLogger("......foo......bar......"), logger_foo_bar);
}


TEST(Logger, same_instance) {

    LoggerRegistryPurge();

    auto logger_root_1 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_1 != nullptr);
    EXPECT_STREQ(logger_root_1->GetName().c_str(), "<root>");
    auto logger_root_2 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_2 != nullptr);
    EXPECT_STREQ(logger_root_2->GetName().c_str(), "<root>");
    EXPECT_TRUE(logger_root_1 == logger_root_2);

    auto logger_foo_1 = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo_1 != nullptr);
    EXPECT_STREQ(logger_foo_1->GetName().c_str(), "foo");
    auto logger_foo_2 = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo_2 != nullptr);
    EXPECT_STREQ(logger_foo_2->GetName().c_str(), "foo");
    EXPECT_TRUE(logger_foo_1 == logger_foo_2);

    auto logger_foo_bar_1 = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar_1 != nullptr);
    EXPECT_STREQ(logger_foo_bar_1->GetName().c_str(), "foo.bar");
    auto logger_foo_bar_2 = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar_2 != nullptr);
    EXPECT_STREQ(logger_foo_bar_2->GetName().c_str(), "foo.bar");
    EXPECT_TRUE(logger_foo_bar_1 == logger_foo_bar_2);

    EXPECT_FALSE(logger_root_1 == logger_foo_1);
    EXPECT_FALSE(logger_root_1 == logger_foo_2);
    EXPECT_FALSE(logger_root_1 == logger_foo_bar_1);
    EXPECT_FALSE(logger_root_1 == logger_foo_bar_2);
    EXPECT_FALSE(logger_root_2 == logger_foo_1);
    EXPECT_FALSE(logger_root_2 == logger_foo_2);
    EXPECT_FALSE(logger_root_2 == logger_foo_bar_1);
    EXPECT_FALSE(logger_root_2 == logger_foo_bar_2);
    EXPECT_FALSE(logger_foo_1 == logger_foo_bar_1);
    EXPECT_FALSE(logger_foo_1 == logger_foo_bar_2);
    EXPECT_FALSE(logger_foo_2 == logger_foo_bar_1);
    EXPECT_FALSE(logger_foo_2 == logger_foo_bar_2);
}


TEST(Logger, ancestors) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");
    std::list<std::string> ancestors{};
    EXPECT_TRUE(logger->GetAncestors() == ancestors);

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);
    EXPECT_STREQ(logger_foo->GetName().c_str(), "foo");
    std::list<std::string> ancestors_foo = {""};
    EXPECT_TRUE(logger_foo->GetAncestors() == ancestors_foo);

    auto logger_foo_bar = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar != nullptr);
    EXPECT_STREQ(logger_foo_bar->GetName().c_str(), "foo.bar");
    std::list<std::string> ancestors_foo_bar = {"foo", ""};
    EXPECT_TRUE(logger_foo_bar->GetAncestors() == ancestors_foo_bar);

    auto logger_foo_bar_baz = headcode::logger::Logger::GetLogger("foo.bar.baz");
    ASSERT_TRUE(logger_foo_bar_baz != nullptr);
    EXPECT_STREQ(logger_foo_bar_baz->GetName().c_str(), "foo.bar.baz");
    std::list<std::string> ancestors_foo_bar_baz = {"foo.bar", "foo", ""};
    EXPECT_TRUE(logger_foo_bar_baz->GetAncestors() == ancestors_foo_bar_baz);

    auto logger_bar = headcode::logger::Logger::GetLogger("bar");
    ASSERT_TRUE(logger_bar != nullptr);
    EXPECT_STREQ(logger_bar->GetName().c_str(), "bar");
    std::list<std::string> ancestors_bar = {""};
    EXPECT_TRUE(logger_bar->GetAncestors() == ancestors_bar);

    auto logger_bar_baz = headcode::logger::Logger::GetLogger("bar.baz");
    ASSERT_TRUE(logger_bar_baz != nullptr);
    EXPECT_STREQ(logger_bar_baz->GetName().c_str(), "bar.baz");
    std::list<std::string> ancestors_bar_baz = {"bar", ""};
    EXPECT_TRUE(logger_bar_baz->GetAncestors() == ancestors_bar_baz);
}

TEST(Logger, parent_sink) {

    LoggerRegistryPurge();

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);

    if (std::filesystem::exists("a.log")) {
        std::filesystem::remove("a.log");
    }

    auto sink = headcode::logger::SinkFactory::Create("file:a.log");
    logger->SetSink(sink);
    auto formatter = std::make_unique<headcode::logger::SimpleFormatter>();
    sink->SetFormatter(std::move(formatter));

    auto logger_foo = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo != nullptr);

    logger_foo->SetBarrier(headcode::logger::Level::kDebug);
    headcode::logger::Debug("foo") << "This is an empty sink.";

    std::ifstream log_file;
    log_file.open("a.log", std::ios::in);

    std::string line;
    std::getline(log_file, line);
    log_file.close();

    EXPECT_STREQ(line.c_str(), "This is an empty sink.");
    EXPECT_EQ(logger->GetEventsLogged(), 0ul);
    EXPECT_GT(logger_foo->GetEventsLogged(), 0ul);
}

#endif
