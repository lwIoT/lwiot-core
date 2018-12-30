#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


BUILD_DIR = `pwd`/../../build/esp32-debug/source
PLATFORM_DIR = $(BUILD_DIR)/platform/esp32
COMPONENT_ADD_LDFLAGS += -L$(BUILD_DIR) -L$(PLATFORM_DIR) -llwiot -llwiot-platform
#COMPONENT_ADD_INCLUDEDIRS += $(BUILD_DIR) $(LWIOT_PATH)/include $(FREERTOS_PATH) $(PORT_PATH)
