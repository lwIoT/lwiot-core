find_path(MBEDTLS_INCLUDE_DIRS mbedtls/ssl.h)

SET(PORT_INCLUDE_DIR
	${PROJECT_SOURCE_DIR}/source/platform/ports/unix
	${PROJECT_SOURCE_DIR}/source/platform/hosted/include
)

SET(PLATFORM_DIRECTORY ${PROJECT_SOURCE_DIR}/source/platform/unix)

find_package(Threads REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/external/freertos/lib)

SET(UNIX True)
SET(LWIOT_SYSTEM_LIBS ${LWIOT_SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} mbedtls mbedx509 mbedcrypto)

SET(HAVE_JSON True CACHE BOOL "Build JSON library")
SET(HAVE_NETWORKING True)
SET(HAVE_SYNC_FETCH True)

SET(PORT_C_FLAGS "-fstack-protector")
SET(PORT_CXX_FLAGS "-fstack-protector")
