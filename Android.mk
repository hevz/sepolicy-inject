LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy-inject-library
LOCAL_CFLAGS :=
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/libsepol/include

LOCAL_SRC_FILES := \
	sepolicy-inject/sepolicy-inject.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy-inject-jni

LOCAL_SRC_FILES := \
	sepolicy-inject/jni.c

LOCAL_STATIC_LIBRARIES := \
	sepolicy-inject-library \
	libsepol

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := sepolicy-inject

LOCAL_STATIC_LIBRARIES := \
	sepolicy-inject-library \
	libsepol

include $(BUILD_EXECUTABLE)

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, libsepol)
