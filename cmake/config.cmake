
if(NOT NO_OS)
	SET(CONFIG_STACK_SIZE 2048 CACHE STRING "RTOS stack size.")
	SET(CONFIG_TASK_PRIO  8 CACHE STRING "Task priority.")
endif()

SET(HAVE_DEBUG False CACHE BOOL "Enable debug output.")
SET(CONFIG_BUILD_TESTS False CACHE BOOL "Build unit tests.")
SET(CONFIG_PIN_VECTOR False CACHE BOOL "Build a vector of pins the the GPIO chip.")