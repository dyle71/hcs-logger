/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/logger.hpp>

#include <gtest/gtest.h>


TEST(Level, regular) {
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kUndefined).empty());
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kSilent).empty());
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kCritical).empty());
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kWarning).empty());
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kInfo).empty());
    EXPECT_FALSE(headcode::logger::GetLevelText(headcode::logger::Level::kDebug).empty());
}


TEST(Level, range_cap) {

    auto undefined_text = headcode::logger::GetLevelText(headcode::logger::Level::kUndefined);
    auto debug_text = headcode::logger::GetLevelText(headcode::logger::Level::kDebug);

    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(-2)).c_str(),
                 undefined_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(-10)).c_str(),
                 undefined_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(-1000)).c_str(),
                 undefined_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(-1'000'000)).c_str(),
                 undefined_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(10)).c_str(), debug_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(1000)).c_str(),
                 debug_text.c_str());
    EXPECT_STREQ(headcode::logger::GetLevelText(static_cast<headcode::logger::Level>(1'000'000)).c_str(),
                 debug_text.c_str());
}
