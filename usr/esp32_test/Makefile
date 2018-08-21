#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32-test

PORT_PATH = $(LWIOT_PATH)/source/ports/freertos
FREERTOS_PATH = $(IDF_PATH)/components/freertos/include/freertos
BUILD_DIR = $(LWIOT_PATH)/build/esp32-release

EXTRA_CFLAGS = -I$(BUILD_DIR) -I$(LWIOT_PATH)/include -I$(FREERTOS_PATH) -I$(PORT_PATH) \
	       -I$(LWIOT_PATH)/include/asm/esp32
EXTRA_CXXFLAGS = $(EXTRA_CFLAGS)

include $(IDF_PATH)/make/project.mk

