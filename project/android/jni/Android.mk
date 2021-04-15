LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := cacu

FILES_SUFFIX := %.cpp %.c

INTERNAL_FILES_PATH  :=  $(LOCAL_PATH) \
					$(LOCAL_PATH)/../../../h3 \
					$(LOCAL_PATH)/../../../app \
					$(LOCAL_PATH)/../../../third_party/android
					
INTERNAL_All_Files := $(foreach src_path,$(INTERNAL_FILES_PATH), $(shell find $(src_path) -type f) ) 
INTERNAL_All_Files := $(INTERNAL_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
INTERNAL_SRC_LIST  := $(filter $(FILES_SUFFIX),$(INTERNAL_All_Files)) 
INTERNAL_SRC_LIST  := $(INTERNAL_SRC_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES := $(INTERNAL_SRC_LIST)
				
INTERNAL_INCLUDE_ROOT := $(LOCAL_PATH)/../../../h3\
							$(LOCAL_PATH)/../../../app
INTERNAL_INCLUDE_DIRS := $(shell find $(INTERNAL_INCLUDE_ROOT) -type d)
LOCAL_C_INCLUDES := $(INTERNAL_INCLUDE_DIRS)

EXTERNAL_FILES_PATH  := $(LOCAL_PATH)/../../../third_party/android\
						$(LOCAL_PATH)/../../../third_party/stb

EXTERNAL_All_Files := $(foreach src_path,$(EXTERNAL_All_Files), $(shell find $(src_path) -type f) ) 
EXTERNAL_All_Files := $(EXTERNAL_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
EXTERNAL_SRC_LIST  := $(filter $(FILES_SUFFIX),$(EXTERNAL_All_Files)) 
EXTERNAL_SRC_LIST  := $(EXTERNAL_SRC_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(EXTERNAL_SRC_LIST)

EXTERNAL_INCLUDE_ROOT := $(LOCAL_PATH)/../../../third_party/android	
EXTERNAL_INCLUDE_DIRS := $(shell find $(EXTERNAL_INCLUDE_ROOT) -type d)
LOCAL_C_INCLUDES += $(EXTERNAL_INCLUDE_DIRS)

LOCAL_LDFLAGS = ../../../third_party/android/prebuilt/libpng/libs/$(TARGET_ARCH_ABI)/libpng.a

LOCAL_CFLAGS := -std=c++11 -fpermissive -D_FILE_OFFSET_BITS=64

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS += -DHAVE_MACHINE=1
endif

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -lz

#build for dynamic library
#include $(BUILD_SHARED_LIBRARY)

#build for static library
include $(BUILD_STATIC_LIBRARY)