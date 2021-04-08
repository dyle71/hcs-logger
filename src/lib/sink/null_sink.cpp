/*
 * This file is part of the headcode.space logger.
 *
 * The 'LICENSE.txt' file in the project root holds the software license.
 * Copyright (C) 2021 headcode.space e.U.
 * Oliver Maurhart <info@headcode.space>, https://www.headcode.space
 */

#include "null_sink.hpp"

#include <atomic>
#include <memory>

using namespace headcode::logger;


void NullSink::RegisterProducer() {
    static std::atomic_flag registered = ATOMIC_FLAG_INIT;
    if (!registered.test_and_set()) {
        SinkFactory::Register(std::make_unique<NullSink::Producer>());
    }
}
