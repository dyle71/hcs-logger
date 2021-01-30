/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger_core.hpp>


TEST(Logger, empty) {
    auto logger = headcode::logger::Logger::GetLogger({});
    EXPECT_TRUE(logger != nullptr);
}


TEST(Logger, same_instance) {

    auto logger_root_1 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_1 != nullptr);
    EXPECT_TRUE(logger_root_1->GetName().empty());
    auto logger_root_2 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_2 != nullptr);
    EXPECT_TRUE(logger_root_2->GetName().empty());
    EXPECT_TRUE(logger_root_1.get() == logger_root_2.get());

    auto logger_foo_1 = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo_1 != nullptr);
    EXPECT_STREQ(logger_foo_1->GetName().c_str(), "foo");
    auto logger_foo_2 = headcode::logger::Logger::GetLogger("foo");
    ASSERT_TRUE(logger_foo_2 != nullptr);
    EXPECT_STREQ(logger_foo_2->GetName().c_str(), "foo");
    EXPECT_TRUE(logger_foo_1.get() == logger_foo_2.get());

    auto logger_foo_bar_1 = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar_1 != nullptr);
    EXPECT_STREQ(logger_foo_bar_1->GetName().c_str(), "foo.bar");
    auto logger_foo_bar_2 = headcode::logger::Logger::GetLogger("foo.bar");
    ASSERT_TRUE(logger_foo_bar_2 != nullptr);
    EXPECT_STREQ(logger_foo_bar_2->GetName().c_str(), "foo.bar");
    EXPECT_TRUE(logger_foo_bar_1.get() == logger_foo_bar_2.get());

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
