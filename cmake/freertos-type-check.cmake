#
# Check whether the required types are available or not
#

if( NOT AVR )
SET(CMAKE_REQUIRED_INCLUDES ${PORT_INCLUDE_DIR})
SET(CMAKE_EXTRA_INCLUDE_FILES FreeRTOS.h task.h queue.h semphr.h timers.h)
CHECK_INCLUDE_FILES (stdbool.h  HAVE_STDBOOL_H)

CHECK_C_SOURCE_COMPILES (
        "
    #include <FreeRTOS.h>
    #include <timers.h>
    int main(int argc, char* argv[])
    {
        TickType_t tick;
        (void)tick;
        return 0;
    }
  " HAVE_TICKTYPE_T)

CHECK_C_SOURCE_COMPILES (
        "
    #include <FreeRTOS.h>
    #include <queue.h>
    int main(int argc, char* argv[])
    {
        QueueHandle_t qh;
        (void)qh;
        return 0;
    }
  " HAVE_QUEUEHANDLE_T)

CHECK_C_SOURCE_COMPILES (
        "
    #include <FreeRTOS.h>
    #include <semphr.h>
    int main(int argc, char* argv[])
    {
        SemaphoreHandle_t sh;
        (void)sh;
        return 0;
    }
  " HAVE_SEMAPHOREHANDLE_T)

CHECK_C_SOURCE_COMPILES (
        "
    #include <FreeRTOS.h>
    #include <timers.h>
    int main(int argc, char* argv[])
    {
        TimerHandle_t tick;
        (void)tick;
        return 0;
    }
  " HAVE_TIMERHANDLE_T)

CHECK_C_SOURCE_COMPILES (
        "
    #include <FreeRTOS.h>
    #include <task.h>
    int main(int argc, char* argv[])
    {
        TaskHandle_t tsk;
        (void)tsk;
        return 0;
    }
  " HAVE_TASKHANDLE_T)
else()
	SET(HAVE_TICKTYPE_T True)
	SET(HAVE_QUEUEHANDLE_T True)
	SET(HAVE_SEMAPHOREHANDLE_T True)
	SET(HAVE_TIMERHANDLE_T True)
	SET(HAVE_TASKHANDLE_T True)
endif()
