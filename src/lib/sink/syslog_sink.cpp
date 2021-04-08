/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include "syslog_sink.hpp"

#include <headcode/logger/event.hpp>
#include <headcode/logger/formatter.hpp>

#include <syslog.h>

using namespace headcode::logger;


SyslogSink::SyslogSink() : Sink{"syslog:"} {
    SetFormatter(std::make_unique<SimpleFormatter>());
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


void SyslogSink::RegisterProducer() {
    static std::atomic_flag registered = ATOMIC_FLAG_INIT;
    if (!registered.test_and_set()) {
        SinkFactory::Register(std::make_unique<SyslogSink::Producer>());
    }
}
