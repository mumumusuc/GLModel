LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  
LOCAL_MODULE := GLRender   
LOCAL_SRC_FILES := GLRender.cpp ModelLoader.cpp com_mumu_glmodel_GLRender.cpp
LOCAL_CPP_INCLUDES += $(LOCAL_PATH) :$(CPP_INCLUDE_PATH)
LOCAL_LDLIBS += -llog -landroid -lEGL -lGLESv3 -ljnigraphics
include $(BUILD_SHARED_LIBRARY)  