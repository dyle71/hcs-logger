# ------------------------------------------------------------
# This file is the main CMakeLists of logger of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include;${TEST_BASE_DIR};${CMAKE_BINARY_DIR})
set(BENCHMARK_TEST_SRC
    test_benchmark.cpp
)

add_executable(benchmark-tests ${BENCHMARK_TEST_SRC} ${BENCHMARK_TEST_OPENSSL_SRC})
target_link_libraries(benchmark-tests hcs-logger ${CONAN_LIBS_GTEST} ${CMAKE_REQUIRED_LIBRARIES})
