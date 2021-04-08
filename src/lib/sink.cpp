/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2020-2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include <headcode/logger/sink.hpp>

#include <headcode/logger/event.hpp>
#include <headcode/logger/formatter.hpp>

#include <headcode/url/url.hpp>

using namespace headcode::logger;
using namespace headcode::url;


Sink::Sink(std::string url) : url_{std::move(url)}, formatter_{std::make_unique<StandardFormatter>()} {
}


Sink::~Sink() = default;


std::string Sink::Format(Event const & event) {
    return formatter_->Format(event);
}


std::string Sink::GetDescription() const {
    return GetDescription_();
}


void Sink::Log(Event const & event) {
    auto level = event.GetLevel();
    if ((level > 0) && (level <= GetBarrier())) {
        events_logged_++;
        Log_(event);
    }
}


void Sink::SetBarrier(int barrier) {
    if (barrier < 0) {
        return;
    }
    barrier_ = barrier;
}


void Sink::SetFormatter(std::unique_ptr<Formatter> && formatter) {
    if (formatter.get() != nullptr) {
        formatter_.reset();
        formatter_.swap(formatter);
    }
}


void Sink::SetBarrier(Level barrier) {
    SetBarrier(static_cast<int>(barrier));
}
