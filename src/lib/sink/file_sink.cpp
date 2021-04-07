/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include "file_sink.hpp"

#include <headcode/logger/formatter.hpp>

#include <headcode/url/url.hpp>

#include <fstream>

using namespace headcode::logger;
using namespace headcode::url;


FileSink::FileSink(std::string file_url) : MutexSink{file_url} {
    URL url{file_url};
    if (url.IsValid() && (url.GetScheme() == "file")) {
        filename_ = url.GetPath().empty() ? "a.log" : url.GetPath();
        if (url.GetPath().empty()) {
            SetURL(std::string{"file:a.log"});
        }
    }
}


std::string FileSink::GetDescription_() const {
    return std::string{"FileSink to "} + filename_;
}


void FileSink::Log_(Event const & event) {

    if (filename_.empty()) {
        return;
    }

    auto lock = LockWrite();
    std::ofstream stream;
    stream.open(filename_, std::ios::out | std::ios::app);
    stream << Format(event);
    stream.flush();
}
