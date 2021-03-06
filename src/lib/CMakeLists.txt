# ------------------------------------------------------------
# This file is part of logger of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

set(LOGGER_SRC

    event.cpp
    formatter.cpp
    level.cpp
    logger.cpp
    sink.cpp
    sink_factory.cpp

    formatter/color_dark_background_formatter.cpp
    formatter/simple_formatter.cpp
    formatter/standard_formatter.cpp

    sink/console_sink.cpp
    sink/file_sink.cpp
    sink/null_sink.cpp
    sink/syslog_sink.cpp
)

add_library(hcs-logger STATIC ${LOGGER_SRC})
target_link_libraries(hcs-logger ${CMAKE_REQUIRED_LIBRARIES})
set_target_properties(hcs-logger PROPERTIES VERSION ${VERSION})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/headcode/logger DESTINATION include COMPONENT header)
install(TARGETS hcs-logger ARCHIVE DESTINATION lib COMPONENT library)
