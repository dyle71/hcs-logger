/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Event, empty) {
    auto event = headcode::logger::Event{0};
    EXPECT_EQ(event.GetLevel(), 0);
    EXPECT_TRUE(event.GetMessage().empty());
}


TEST(Event, regular) {

    auto event = headcode::logger::Event{headcode::logger::Level::kInfo};
    event << "The quick brown fox jumped over the lazy dog.";

    EXPECT_EQ(event.GetLevel(), static_cast<int>(headcode::logger::Level::kInfo));
    EXPECT_STREQ(event.GetMessage().c_str(), "The quick brown fox jumped over the lazy dog.");
    EXPECT_EQ(event.GetLogger(), headcode::logger::Logger::GetLogger());
    EXPECT_GT(event.GetAge().count(), 0);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(event.GetTimePoint().time_since_epoch());
    EXPECT_NE(milliseconds.count(), 0);
}
