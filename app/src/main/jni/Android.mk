LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

OPENCV_INSTALL_MODULES:=on

OPENCV_LIB_TYPE:=STATIC

include /Users/ye/OpenCV-android-sdk2_4_11/sdk/native/jni/OpenCV.mk

LOCAL_MODULE := shapedetection
LOCAL_C_INCLUDES := /Users/ye/OpenCV-android-sdk2_4_11/sdk/native/jni/include
LOCAL_SRC_FILES := shape_detection.cpp


include $(BUILD_SHARED_LIBRARY)