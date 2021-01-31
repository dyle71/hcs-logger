/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(StringFormatter, empty) {
    auto log = headcode::logger::StringFormatter{}.Format(headcode::logger::Event{0});
    EXPECT_TRUE(log.empty());
}


TEST(StringFormatter, regular) {

    std::string expected{"The quick brown fox jumps over the lazy dog."};

    headcode::logger::Event event{0};
    event << "The quick brown fox jumps over the lazy dog.";

    auto log = headcode::logger::StringFormatter{}.Format(event);
    EXPECT_STREQ(log.c_str(), expected.c_str());
}
