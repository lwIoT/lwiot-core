add_subdirectory(ports)
add_subdirectory(network)

if(NOT HAVE_RTOS)
	add_subdirectory(generic)
endif()
