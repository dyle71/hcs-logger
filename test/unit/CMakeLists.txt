# ------------------------------------------------------------
# This file is the main CMakeLists of logger of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include;${TEST_BASE_DIR};${CMAKE_BINARY_DIR})
set(UNIT_TEST_SRC
    test_event.cpp
    test_formatter.cpp
    test_level.cpp
    test_logger.cpp
    test_sink.cpp
    test_threading.cpp
    test_version.cpp
)

add_executable(unit-tests ${UNIT_TEST_SRC} ${UNIT_TEST_OPENSSL_SRC})
target_link_libraries(unit-tests hcs-logger ${CONAN_LIBS_GTEST} ${CMAKE_REQUIRED_LIBRARIES})
gtest_add_tests(unit-tests "" AUTO)
