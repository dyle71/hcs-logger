/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Logger, empty) {
    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");
}


TEST(Logger, regular) {

    auto logger = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger != nullptr);
    EXPECT_STREQ(logger->GetName().c_str(), "<root>");

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

    auto logger_root_1 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_1 != nullptr);
    EXPECT_STREQ(logger_root_1->GetName().c_str(), "<root>");
    auto logger_root_2 = headcode::logger::Logger::GetLogger({});
    ASSERT_TRUE(logger_root_2 != nullptr);
    EXPECT_STREQ(logger_root_2->GetName().c_str(), "<root>");
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


TEST(Logger, ancestors) {

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
