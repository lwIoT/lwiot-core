find_path(MBEDTLS_INCLUDE_DIRS mbedtls/ssl.h)

SET(UNIX True)
SET(LWIOT_SYSTEM_LIBS ${LWIOT_SYSTEM_LIBS} ${CMAKE_THREAD_LIBS_INIT} mbedtls mbedx509 mbedcrypto)

SET(HAVE_JSON True CACHE BOOL "Build JSON library")
SET(HAVE_NETWORKING True)
SET(HAVE_SYNC_FETCH True)

SET(PORT_C_FLAGS "-fstack-protector -Wextra -Wno-error=unused-function -Wno-error=unused-but-set-variable \
	-Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare \
	 -Wno-implicit-fallthrough -fno-inline-functions")
SET(PORT_CXX_FLAGS "-fstack-protector -Wextra -fno-rtti -Wno-error=unused-function -Wno-error=unused-but-set-variable \
 -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter \
 -Wno-implicit-fallthrough -Wno-sign-compare -fno-inline-functions")
