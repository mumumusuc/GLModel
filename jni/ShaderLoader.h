#include <stdlib.h>
#include <stdio.h>
#include <GLES3/gl3.h>
#include <android/log.h>

#define LOG_TAG "NDK_ShaderLoader"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

GLuint loadShader(GLenum, const char*);

#ifdef __cplusplus
}
#endif
