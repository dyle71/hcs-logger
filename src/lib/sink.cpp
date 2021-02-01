/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/sink.hpp>


using namespace headcode::logger;


std::string Sink::Format(Event const & event) {
    // TODO: Apply formatter here.
    return event.str();
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


void Sink::SetBarrier(Level barrier) {
    SetBarrier(static_cast<int>(barrier));
}


void NullSink::Log_(Event const &) {
}


void StreamSink::Log_(Event const & event) {
    stream_ << Format(event);
    stream_.flush();
}
