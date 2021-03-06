cmake_minimum_required(VERSION 3.0)
project(lwiot-core)

include(CMakeToolsHelpers OPTIONAL)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(TestBigEndian)
include(${PROJECT_SOURCE_DIR}/cmake/config.cmake)

if(UNIX)
	include(${PROJECT_SOURCE_DIR}/cmake/unix.cmake)
elseif(WIN32)
    include(${PROJECT_SOURCE_DIR}/cmake/win32.cmake)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(HAVE_DEBUG True)
endif ()

if(UNIX OR CMAKE_SYSTEM_NAME MATCHES Generic)
    if(BUILD_TYPE MATCHES Native)
        SET(CFLAGS_EXTRA "-march=native -mtune=native")
    endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS_EXTRA} ${PORT_C_FLAGS} -Wall -std=gnu11")
else()
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS_EXTRA} ${PORT_C_FLAGS}")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS_EXTRA} ${PORT_CXX_FLAGS} -fstrict-enums -Wall -Wno-varargs")
else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS_EXTRA} ${PORT_CXX_FLAGS}")
endif()

    if(UNIX)
        TEST_BIG_ENDIAN(HAVE_BIG_ENDIAN)
    endif()
endif()

if(WIN32)
	TEST_BIG_ENDIAN(HAVE_BIG_ENDIAN)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        SET(CFLAGS_EXTRA "/W4 /Od /Ob1 /Zi /Gd")
    elseif(CMAKE_BUILD_TYPE MATCHES Release)
        SET(CFLAGS_EXTRA "/W4 /O2 /Ob2 /Gd /Oi")
    endif()

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS_EXTRA}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS_EXTRA}")
endif()

# Check header files
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (stdio.h  HAVE_STDIO_H)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (stdarg.h HAVE_STDARG_H)
CHECK_INCLUDE_FILES (assert.h HAVE_ASSERT_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (sys/unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILES (sys/stat.h HAVE_STAT_H)
CHECK_INCLUDE_FILES (time.h HAVE_TIME_H)
CHECK_INCLUDE_FILES (Winsock2.h HAVE_WINSOCK_H)
CHECK_INCLUDE_FILES (arpa/inet.h HAVE_INET_H)

CHECK_C_SOURCE_COMPILES (
        "
    #include <stdlib.h>
    #include <stdint.h>
    int main(int argc, char* argv[])
    {
        ssize_t size;
        (void)size;
        return 0;
    }
  " HAVE_SSIZE_T)

CHECK_C_SOURCE_COMPILES (
        "
    #include <stdlib.h>
    #include <stdint.h>
    int main(int argc, char* argv[])
    {
        size_t size;
        (void)size;
        return 0;
    }
  " HAVE_SIZE_T)

if(CMAKE_SYSTEM_NAME MATCHES Generic)
    SET(HAVE_GENERIC_SYS True)
endif()


if(WIN32 OR UNIX)
    add_subdirectory(scripts)

    if(CONFIG_BUILD_TESTS)
        add_subdirectory(tests)
    endif()
endif()

INSTALL(PROGRAMS scripts/avr_upload.rb DESTINATION bin RENAME avr_upload)
INSTALL(PROGRAMS scripts/spiffsgen.py DESTINATION scripts RENAME spiffsgen.py)

add_subdirectory(source)
