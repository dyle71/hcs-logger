# ------------------------------------------------------------
# This file is part of crypt of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.  
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

include_directories(${CMAKE_SOURCE_DIR}/include)

set(TEST_SRC
    test.cpp
)

add_executable(test-console ${TEST_SRC})
target_link_libraries(test-console "hcs-logger;${CMAKE_REQUIRED_LIBRARIES}")
add_test(integration/console ${CMAKE_CURRENT_BINARY_DIR}/test-console)
