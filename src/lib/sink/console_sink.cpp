/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include "console_sink.hpp"

#include <headcode/logger/formatter.hpp>
#include <headcode/url/url.hpp>

#include <unistd.h>

#include <atomic>
#include <iostream>

using namespace headcode::logger;
using namespace headcode::url;


/**
 * @brief   All ConsoleSinks storage.
 */
std::map<std::string, std::shared_ptr<ConsoleSink>> ConsoleSink::Producer::sinks;


/**
 * @brief   All ConsoleSinks access synchronization mutex.
 */
std::mutex ConsoleSink::Producer::mutex;


ConsoleSink::ConsoleSink(std::string stream_url) : MutexSink{stream_url} {

    int stream_fd = 0;
    URL url{stream_url};
    if (url.IsValid()) {
        if (url.GetScheme() == "stdout") {
            out_ = &std::cout;
            stream_fd = 1;
        } else if (url.GetScheme() == "stderr") {
            out_ = &std::cerr;
            stream_fd = 2;
        }
    }

    if (stream_fd != 0 && isatty(stream_fd) == 1) {
        SetFormatter(std::make_unique<ColorDarkBackgroundFormatter>());
    } else {
        SetFormatter(std::make_unique<StandardFormatter>());
    }
}


std::string ConsoleSink::GetDescription_() const {
    return std::string{"ConsoleSink to "} + GetURL();
}


void ConsoleSink::Log_(Event const & event) {
    if (out_ != nullptr) {
        auto lock = LockWrite();
        *out_ << Format(event);
        out_->flush();
    }
}


void ConsoleSink::RegisterProducer() {
    static std::atomic_flag registered = ATOMIC_FLAG_INIT;
    if (!registered.test_and_set()) {
        SinkFactory::Register(std::make_unique<ConsoleSink::Producer>());
    }
}
