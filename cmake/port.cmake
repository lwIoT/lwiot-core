if(WIN32)
#include (${PROJECT_SOURCE_DIR}/cmake/win32.cmake)
elseif(UNIX)
include (${PROJECT_SOURCE_DIR}/cmake/unix.cmake)
elseif(FREERTOS)
    include(${PROJECT_SOURCE_DIR}/cmake/freertos-type-check.cmake)
    if(NOT FREERTOS_MANUAL)
        include (${PROJECT_SOURCE_DIR}/cmake/freertos.cmake)
    endif()
endif()

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
