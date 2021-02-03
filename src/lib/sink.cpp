/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/sink.hpp>

#include <iostream>
#include <fstream>
#include <utility>

// Currently very Linux/Unix centered...
#include <syslog.h>
#include <unistd.h>

#include <headcode/logger/formatter.hpp>

using namespace headcode::logger;


std::mutex headcode::logger::ConsoleSink::console_mutex_;


Sink::Sink() : formatter_{std::make_shared<StandardFormatter>()} {
}


std::string Sink::Format(Event const & event) {
    return formatter_->Format(event);
}


std::string Sink::GetDescription() const {
    return GetDescription_();
}


void Sink::Log(Event const & event) {
    auto level = event.GetLevel();
    if ((level > 0) && (level <= GetBarrier())) {
        Log_(event);
    }
}


void Sink::SetBarrier(int barrier) {
    if (barrier < 0) {
        return;
    }
    barrier_ = barrier;
}


void Sink::SetFormatter(std::shared_ptr<Formatter> const & formatter) {
    if (formatter != nullptr) {
        formatter_ = formatter;
    }
}

void Sink::SetBarrier(Level barrier) {
    SetBarrier(static_cast<int>(barrier));
}


void NullSink::Log_(Event const &) {
}


FileSink::FileSink(std::string filename) : Sink{}, filename_{std::move(filename)} {
    if (filename_.empty()) {
        filename_ = "a.log";
    }
}


std::string FileSink::GetDescription_() const {
    return std::string{"FileSink to "} + filename_;
}


void FileSink::Log_(Event const & event) {

    std::lock_guard<std::mutex> lock{mutex_};

    // We may keep the file open for a better performance. I have to test.
    std::ofstream stream;
    stream.open(filename_, std::ios::out | std::ios::app);
    stream << Format(event);
    stream.flush();
}


ConsoleSink::ConsoleSink() : Sink{} {
    if (isatty(2) == 1) {
        SetFormatter(std::make_shared<ColorDarkBackgroundFormatter>());
    }
}


std::string ConsoleSink::GetDescription_() const {
    return std::string{"ConsoleSink"};
}


void ConsoleSink::Log_(Event const & event) {
    std::lock_guard<std::mutex> lock{console_mutex_};
    std::cerr << Format(event);
    std::cerr.flush();
}


SyslogSink::SyslogSink() : Sink{} {
    SetFormatter(std::make_shared<SimpleFormatter>());
}


std::string SyslogSink::GetDescription_() const {
    return std::string{"SyslogSink"};
}


void SyslogSink::Log_(Event const & event) {

    // We may keep the syslog open for a better performance. I have to test.
    openlog(nullptr, LOG_PID, LOG_USER);

    int priority;
    switch (event.GetLevel()) {

        case static_cast<int>(Level::kCritical):
            priority = LOG_CRIT;
            break;

        case static_cast<int>(Level::kWarning):
            priority = LOG_WARNING;
            break;

        case static_cast<int>(Level::kInfo):
            priority = LOG_INFO;
            break;

        case static_cast<int>(Level::kDebug):
            priority = LOG_DEBUG;
            break;

        default:
            priority = LOG_ERR;
    }

    syslog(priority, "%s", Format(event).c_str());
    closelog();
}
