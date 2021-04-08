/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger.hpp>

#include <gtest/gtest.h>


TEST(Event, empty) {
    auto event = headcode::logger::Event{0};
    EXPECT_EQ(event.GetLevel(), 0);
    EXPECT_TRUE(event.GetMessage().empty());
}


TEST(Event, regular) {

    auto event = headcode::logger::Event{headcode::logger::Level::kInfo};
    event << "The quick brown fox jumped over the lazy dog." << std::endl;

    EXPECT_EQ(event.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    EXPECT_STREQ(event.GetMessage().c_str(), "The quick brown fox jumped over the lazy dog.\n");
    EXPECT_EQ(event.GetLogger(), headcode::logger::Logger::GetLogger());
    EXPECT_GT(event.GetAge().count(), 0);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(event.GetTimePoint().time_since_epoch());
    EXPECT_NE(milliseconds.count(), 0);

    auto debug = headcode::logger::Debug();
    EXPECT_EQ(debug.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));

    auto debug_root_name = headcode::logger::Debug("");
    EXPECT_EQ(debug_root_name.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));
    EXPECT_EQ(debug_root_name.GetLogger(), headcode::logger::Logger::GetLogger(""));

    auto debug_root_ptr = headcode::logger::Debug(headcode::logger::Logger::GetLogger("foo"));
    EXPECT_EQ(debug_root_ptr.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));
    EXPECT_EQ(debug_root_ptr.GetLogger(), headcode::logger::Logger::GetLogger("foo"));

    auto info = headcode::logger::Info();
    EXPECT_EQ(info.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));

    auto info_root_name = headcode::logger::Info("");
    EXPECT_EQ(info_root_name.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    EXPECT_EQ(info_root_name.GetLogger(), headcode::logger::Logger::GetLogger(""));

    auto info_root_ptr = headcode::logger::Info(headcode::logger::Logger::GetLogger("foo"));
    EXPECT_EQ(info_root_ptr.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    EXPECT_EQ(info_root_ptr.GetLogger(), headcode::logger::Logger::GetLogger("foo"));

    auto warning = headcode::logger::Warning();
    EXPECT_EQ(warning.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));

    auto warning_root_name = headcode::logger::Warning("");
    EXPECT_EQ(warning_root_name.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));
    EXPECT_EQ(warning_root_name.GetLogger(), headcode::logger::Logger::GetLogger(""));

    auto warning_root_ptr = headcode::logger::Warning(headcode::logger::Logger::GetLogger("foo"));
    EXPECT_EQ(warning_root_ptr.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));
    EXPECT_EQ(warning_root_ptr.GetLogger(), headcode::logger::Logger::GetLogger("foo"));

    auto critical = headcode::logger::Critical();
    EXPECT_EQ(critical.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));

    auto critical_root_name = headcode::logger::Critical("");
    EXPECT_EQ(critical_root_name.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));
    EXPECT_EQ(critical_root_name.GetLogger(), headcode::logger::Logger::GetLogger(""));

    auto critical_root_ptr = headcode::logger::Critical(headcode::logger::Logger::GetLogger("foo"));
    EXPECT_EQ(critical_root_ptr.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));
    EXPECT_EQ(critical_root_ptr.GetLogger(), headcode::logger::Logger::GetLogger("foo"));
}


TEST(Event, user_numbers) {
    auto event = headcode::logger::Event{1337};
    EXPECT_EQ(event.GetLevel(), 1337);
}


TEST(Event, debug) {

    {
        auto debug = headcode::logger::Debug{};
        EXPECT_EQ(debug.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));
    }
    
    {
        auto debug = headcode::logger::Debug{""};
        EXPECT_EQ(debug.GetLogger()->GetId(), 0u);
        EXPECT_EQ(debug.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));
    }

    {
        auto debug = headcode::logger::Debug{headcode::logger::Logger::GetLogger("foo")};
        EXPECT_STREQ(debug.GetLogger()->GetName().c_str(), "foo");
        EXPECT_EQ(debug.GetLevel(), static_cast<int>(headcode::logger::Level::kDebug));
    }
}


TEST(Event, warning) {

    {
        auto warning = headcode::logger::Warning{};
        EXPECT_EQ(warning.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));
    }

    {
        auto warning = headcode::logger::Warning{""};
        EXPECT_EQ(warning.GetLogger()->GetId(), 0u);
        EXPECT_EQ(warning.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));
    }

    {
        auto warning = headcode::logger::Warning{headcode::logger::Logger::GetLogger("foo")};
        EXPECT_STREQ(warning.GetLogger()->GetName().c_str(), "foo");
        EXPECT_EQ(warning.GetLevel(), static_cast<int>(headcode::logger::Level::kWarning));
    }
}


TEST(Event, info) {

    {
        auto info = headcode::logger::Info{};
        EXPECT_EQ(info.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    }

    {
        auto info = headcode::logger::Info{""};
        EXPECT_EQ(info.GetLogger()->GetId(), 0u);
        EXPECT_EQ(info.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    }

    {
        auto info = headcode::logger::Info{headcode::logger::Logger::GetLogger("foo")};
        EXPECT_STREQ(info.GetLogger()->GetName().c_str(), "foo");
        EXPECT_EQ(info.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    }
}


TEST(Event, critical) {

    {
        auto critical = headcode::logger::Critical{};
        EXPECT_EQ(critical.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));
    }

    {
        auto critical = headcode::logger::Critical{""};
        EXPECT_EQ(critical.GetLogger()->GetId(), 0u);
        EXPECT_EQ(critical.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));
    }

    {
        auto critical = headcode::logger::Critical{headcode::logger::Logger::GetLogger("foo")};
        EXPECT_STREQ(critical.GetLogger()->GetName().c_str(), "foo");
        EXPECT_EQ(critical.GetLevel(), static_cast<int>(headcode::logger::Level::kCritical));
    }
}
