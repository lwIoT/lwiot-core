#
# AVR toolchain file
#

SET(CMAKE_SYSTEM_NAME Generic)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.6.0")
	set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
else()
	include(CMakeForceCompiler)

	set(AVR_GCC_COMPILER "avr-gcc${CMAKE_EXECUTABLE_SUFFIX}")
	set(AVR_GXX_COMPILER "avr-g++${CMAKE_EXECUTABLE_SUFFIX}")
	cmake_force_c_compiler(avr-gcc GNU)
	cmake_force_cxx_compiler(avr-g++ GNU)
endif()

set(HAVE_BIG_ENDIAN False)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(NO_OS False CACHE BOOL "Build without RTOS")
SET(MCU "atmega2560" CACHE STRING "AVR MCU type")

if(NO_OS)
else()
set(PORT_INCLUDE_DIR
	${PROJECT_SOURCE_DIR}/external/avr-freertos/include
	${PROJECT_SOURCE_DIR}/source/platform/avr/include
)
endif()

set(LWIOT_CORE_INCLUDE_DIRECTORIES
	${PROJECT_SOURCE_DIR}/source/lib/time
)

SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/avr)

SET(F_CPU 16000000 CACHE STRING "AVR CPU frequency")

set(PORT_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -fdata-sections -ffunction-sections")
set(PORT_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -fno-rtti -fdata-sections -ffunction-sections -Wc++14-compat")

SET(HAVE_LIBTIME True)
