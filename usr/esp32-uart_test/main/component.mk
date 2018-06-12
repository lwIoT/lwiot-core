#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


BUILD_DIR = /home/michel/cloud/workspace/Sensate/lwIoT/build/esp32/source
COMPONENT_ADD_LDFLAGS += -L$(BUILD_DIR) -llwiot
#COMPONENT_ADD_INCLUDEDIRS += $(BUILD_DIR) $(LWIOT_PATH)/include $(FREERTOS_PATH) $(PORT_PATH)
