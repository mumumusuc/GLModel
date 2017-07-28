LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)  
LOCAL_MODULE := Renderer   
LOCAL_SRC_FILES := GLRender.cpp com_mumu_glmodel_render_Renderer.cpp
LOCAL_CPP_INCLUDES += $(LOCAL_PATH) :$(CPP_INCLUDE_PATH)
LOCAL_LDLIBS += -llog -landroid -lEGL -lGLESv3 -ljnigraphics
include $(BUILD_SHARED_LIBRARY)  

include $(CLEAR_VARS)  
LOCAL_MODULE := ModelLoader   
LOCAL_SRC_FILES := ModelLoader.cpp com_mumu_glmodel_model_ModelLoader.cpp
LOCAL_CPP_INCLUDES += $(LOCAL_PATH) :$(CPP_INCLUDE_PATH)
LOCAL_LDLIBS += -llog -landroid -lEGL -lGLESv3 -ljnigraphics
include $(BUILD_SHARED_LIBRARY)  