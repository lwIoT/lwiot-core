add_subdirectory(ports)

if(NOT HAVE_RTOS)
	add_subdirectory(generic)
	add_subdirectory(network)
endif()
