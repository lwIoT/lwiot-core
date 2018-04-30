cmake_minimum_required(VERSION 3.0)
project(lwIoT)

include(CMakeToolsHelpers OPTIONAL)
include(CheckIncludeFiles)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(TestBigEndian)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(HAVE_DEBUG True)
endif ()

if(UNIX OR CMAKE_SYSTEM_NAME MATCHES Generic)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        SET(CFLAGS_EXTRA "-ggdb -g3")
    endif()

    if(BUILD_TYPE MATCHES Native)
        SET(CFLAGS_EXTRA "-march=native -mtune=native")
    endif()

    if(CMAKE_BUILD_TYPE MATCHES Release)
        set(CFLAGS_EXTRA "${CFLAGS_EXTRA} -ffast-math")
    endif()

    if(CMAKE_BUILD_TYPE MATCHES RelSize)
        set(CFLAGS_EXTRA "${CFLAGS_EXTRA} -Os")
    endif()

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CFLAGS_EXTRA} -Wall")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CFLAGS_EXTRA} -Wall")
	
	if(UNIX)
		TEST_BIG_ENDIAN(HAVE_BIG_ENDIAN)
	endif()
elseif(WIN32)
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

if( NOT CMAKE_SYSTEM_NAME MATCHES Generic OR PORT MATCHES unix )
    add_subdirectory(scripts)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        add_subdirectory(tests)
    endif()
endif()

add_subdirectory(source)
