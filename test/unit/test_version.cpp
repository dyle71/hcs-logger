/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Version, regular) {
    std::uint32_t version = MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
    EXPECT_EQ(headcode::logger::GetCurrentVersion(), version);
    EXPECT_STREQ(headcode::logger::GetVersionString().c_str(), VERSION);
}


TEST(Version, V1_0_0) {
    std::uint32_t version = MAKE_VERSION(1, 0, 0);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (0)), version);
    EXPECT_EQ(GetHCSLoggerVersion_1_0_0(), version);
}


TEST(Version, V1_0_1) {
    std::uint32_t version = MAKE_VERSION(1, 0, 1);
    EXPECT_EQ(static_cast<unsigned int>((1) << 24 | (0) << 16 | (1)), version);
    EXPECT_EQ(GetHCSLoggerVersion_1_0_1(), version);
}
