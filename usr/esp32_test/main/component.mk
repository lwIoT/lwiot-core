#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


BUILD_DIR = `pwd`/../../build/esp32/source
COMPONENT_ADD_LDFLAGS += -L$(BUILD_DIR) -L$(BUILD_DIR)/platform/esp32 -llwiot -llwiot-platform
#COMPONENT_ADD_INCLUDEDIRS += $(BUILD_DIR) $(LWIOT_PATH)/include $(FREERTOS_PATH) $(PORT_PATH)
