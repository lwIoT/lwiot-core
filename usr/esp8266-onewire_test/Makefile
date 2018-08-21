# Makefile for access_point example
PROGRAM=lwiot_test
EXTRA_COMPONENTS=extras/dhcpserver

BASE := $(LWIOT_PATH)
INCLUDE := $(BASE)/include
PORTS := $(BASE)/source/ports/freertos
BIN := $(BASE)/build/esp8266-release
LIBDIR := $(BASE)/build/esp8266-release/source

EXTRA_C_CXX_FLAGS := -I$(INCLUDE) -I$(BASE)/include/asm/esp8266 -I$(PORTS) -I$(BIN)
EXTRA_LDFLAGS = -Wl,-EL,--gc-sections -L$(LIBDIR) -Wl,--whole-archive -llwiot -Wl,--no-whole-archive

include $(ESP_OPEN_RTOS_PATH)/common.mk
