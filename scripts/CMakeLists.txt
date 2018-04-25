
IF(WIN32)
    SET(BOT_SRC bot/byteordertool.c bot/getopt.c bot/getopt.h)
ELSE()
    SET(BOT_SRC bot/byteordertool.c)
ENDIF()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(byteordertool ${BOT_SRC})

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/scripts/ci-runner.py ${CMAKE_BINARY_DIR}/scripts/ci-runner.py COPYONLY)
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/scripts/utest.py ${CMAKE_BINARY_DIR}/scripts/utest.py COPYONLY)
