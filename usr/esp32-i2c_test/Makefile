#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := esp32-test

PORT_PATH = $(LWIOT_PATH)/source/platform/esp32/include
FREERTOS_PATH = $(IDF_PATH)/components/freertos/include/freertos
BUILD_DIR = $(LWIOT_PATH)/build/esp32

EXTRA_CFLAGS = -I$(BUILD_DIR) -I$(LWIOT_PATH)/include -I$(FREERTOS_PATH) -I$(PORT_PATH)
EXTRA_CXXFLAGS = $(EXTRA_CFLAGS)

include $(IDF_PATH)/make/project.mk

