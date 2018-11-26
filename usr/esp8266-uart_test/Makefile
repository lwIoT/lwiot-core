# Makefile for access_point example
PROGRAM=lwiot_test
EXTRA_COMPONENTS=extras/dhcpserver

ESP_OPEN_RTOS_PATH=$(LWIOT_PATH)/external/esp8266-open-rtos
BASE := $(LWIOT_PATH)
INCLUDE := $(BASE)/include
BIN := $(BASE)/build/esp8266
LIBDIR := $(BASE)/build/esp8266/source
PLATFORMDIR := $(BIN)/source/platform/esp8266
PLATFORMDIR_INC := $(BASE)/source/platform/esp8266

EXTRA_C_CXX_FLAGS := -I$(INCLUDE) -I$(BIN) -I$(PLATFORMDIR_INC)/include
EXTRA_LDFLAGS = -L$(LIBDIR) -L$(PLATFORMDIR) -Wl,--whole-archive -llwiot -llwiot-platform -Wl,--no-whole-archive 

include $(ESP_OPEN_RTOS_PATH)/common.mk
