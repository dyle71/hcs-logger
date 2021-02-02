/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <gtest/gtest.h>

#include <headcode/logger/logger.hpp>


TEST(Formatter, message_split) {

    std::string message;
    std::list<std::string> lines;
    std::list<std::string> expected;

    message = "The quick brown fox jumps over the lazy dog.";
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"The quick brown fox jumps over the lazy dog.\n"}};
    EXPECT_EQ(lines.size(), 1u);
    EXPECT_EQ(lines, expected);

    message = std::string{};
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"\n"}};
    EXPECT_EQ(lines.size(), 1u);
    EXPECT_EQ(lines, expected);

    message = "foo\nbar";
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"foo\n"}, {"bar\n"}};
    EXPECT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines, expected);

    message = "foo\nbar\n";
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"foo\n"}, {"bar\n"}};
    EXPECT_EQ(lines.size(), 2u);
    EXPECT_EQ(lines, expected);

    message = "foo\nbar\nbaz";
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"foo\n"}, {"bar\n"}, {"baz\n"}};
    EXPECT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines, expected);

    message = "foo\nbar\nbaz\n";
    lines = headcode::logger::Formatter::SplitMessageIntoLines(message);
    expected = std::list<std::string>{{"foo\n"}, {"bar\n"}, {"baz\n"}};
    EXPECT_EQ(lines.size(), 3u);
    EXPECT_EQ(lines, expected);
}


TEST(Formatter, time_string) {

    headcode::logger::Event event{0};
    auto time_string = headcode::logger::Formatter::CreateTimeString(event);
    EXPECT_EQ(time_string.size(), 31u);
}


TEST(Formatter, level_string) {

    headcode::logger::Debug debug;
    EXPECT_STREQ(headcode::logger::Formatter::CreateLevelString(debug).c_str(), "(debug   )");

    headcode::logger::Info info;
    EXPECT_STREQ(headcode::logger::Formatter::CreateLevelString(info).c_str(), "(info    )");

    headcode::logger::Warning warning;
    EXPECT_STREQ(headcode::logger::Formatter::CreateLevelString(warning).c_str(), "(warning )");

    headcode::logger::Critical critical;
    EXPECT_STREQ(headcode::logger::Formatter::CreateLevelString(critical).c_str(), "(critical)");

    headcode::logger::Event event{1000};
    EXPECT_STREQ(headcode::logger::Formatter::CreateLevelString(event).c_str(), "(debug   )");
}


TEST(Formatter, logger_string) {

    headcode::logger::Event root_event{0};
    EXPECT_TRUE(headcode::logger::Formatter::CreateLoggerString(root_event).empty());

    headcode::logger::Event foo_event{0, "foo"};
    EXPECT_STREQ(headcode::logger::Formatter::CreateLoggerString(foo_event).c_str(), "{foo}");

    headcode::logger::Event foo_bar_event{0, "foo.bar"};
    EXPECT_STREQ(headcode::logger::Formatter::CreateLoggerString(foo_bar_event).c_str(), "{foo.bar}");
}


TEST(SimpleFormatter, empty) {
    auto log = headcode::logger::SimpleFormatter{}.Format(headcode::logger::Event{0});
    EXPECT_TRUE(log.empty());
}


TEST(SimpleFormatter, regular) {

    std::string expected{"The quick brown fox jumps over the lazy dog."};

    headcode::logger::Event event{0};
    event << "The quick brown fox jumps over the lazy dog.";

    auto log = headcode::logger::SimpleFormatter{}.Format(event);
    EXPECT_STREQ(log.c_str(), expected.c_str());
}


TEST(StandardFormatter, regular) {

    std::string log;

    headcode::logger::Event event1{headcode::logger::Level::kDebug};
    event1 << "The quick brown fox jumps over the lazy dog.";
    log = headcode::logger::StandardFormatter{}.Format(event1);
    EXPECT_FALSE(log.empty());
    std::cerr << log;

    headcode::logger::Event event2{headcode::logger::Level::kInfo, "foo"};
    event2 << "The quick brown \nfox jumps over \nthe lazy dog.";
    log = headcode::logger::StandardFormatter{}.Format(event2);
    EXPECT_FALSE(log.empty());
    std::cerr << log;
}
