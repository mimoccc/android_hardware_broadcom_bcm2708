ifneq ($(strip $(TARGET_EGL_PREBUILT)),true)
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(ANDROID_BUILD_TOP)/hardware/broadcom/$(TARGET_BOARD_PLATFORM)/cflags.mk

LOCAL_C_INCLUDES += hardware/broadcom/$(TARGET_BOARD_PLATFORM)/include

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)/egl
LOCAL_MODULE := libEGL_bcm2708
	
include $(LOCAL_PATH)/android-sources.mk

$(info TARGET_LOG_EGL $(TARGET_LOG_EGL))
ifeq ($strip $(TARGET_LOG_EGL),true)
$(info EGL LOGGING ENABLED)
LOCAL_CFLAGS += -DLOG_NDEBUG=0
endif
		
LOCAL_SHARED_LIBRARIES := libvchiq_arm libvcos libm liblog libvchost libhardware
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif
