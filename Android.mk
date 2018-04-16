LOCAL_PATH:= $(call my-dir)

ANDROID_MAJOR :=
ANDROID_MINOR :=
ANDROID_MICRO :=

ifndef ANDROID_MAJOR
include build/core/version_defaults.mk
ifeq ($(strip $(PLATFORM_VERSION)),)
$error(*** Cannot get Android platform version)
endif
ANDROID_MAJOR = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 1)
ANDROID_MINOR = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 2)
ANDROID_MICRO = $(shell echo $(PLATFORM_VERSION) | cut -d . -f 3)
endif

ifeq ($(strip $(ANDROID_MAJOR)),)
$(error *** ANDROID_MAJOR undefined)
endif

ifeq ($(strip $(ANDROID_MINOR)),)
$(error *** ANDROID_MINOR undefined)
endif

ifeq ($(strip $(ANDROID_MICRO)),)
$(warning *** ANDROID_MICRO undefined. Assuming 0)
ANDROID_MICRO = 0
endif

include $(CLEAR_VARS)
LOCAL_SRC_FILES := droidgnss.cpp \
                   private.cpp

LOCAL_C_INCLUDES := frameworks/native/services

LOCAL_SHARED_LIBRARIES := libbinder \
                          libc \
                          libcutils \
                          libdl \
                          libhidlbase \
                          libhidltransport \
                          liblog \
                          libutils \
                          android.hardware.gnss@1.0
LOCAL_CPPFLAGS=-DANDROID_MAJOR=$(ANDROID_MAJOR) -DANDROID_MINOR=$(ANDROID_MINOR) -DANDROID_MICRO=$(ANDROID_MICRO) $(FORCE_HAL_PARAM)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libdroidgnss
ifeq ($(strip $(DROIDSENSORS_32)), true)
LOCAL_MODULE_TARGET_ARCH := arm
endif

include $(BUILD_SHARED_LIBRARY)
