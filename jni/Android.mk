LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  
LOCAL_MODULE := GLRender   
LOCAL_SRC_FILES := GLRender.cpp ShaderLoader.cpp ModelLoader.cpp
LOCAL_CPP_INCLUDES += $(LOCAL_PATH) 
LOCAL_LDLIBS += -llog -landroid -lEGL -lGLESv3 -ljnigraphics
include $(BUILD_SHARED_LIBRARY)  