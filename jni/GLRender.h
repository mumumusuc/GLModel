#include "com_mumu_glmodel_GLRender.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <string.h>
#include <GLES3/gl3.h>
#include "include/vmath.h"
#include "ShaderLoader.h"
#include "ModelLoader.h"

#define LOG_TAG "NDK_GLRender"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ERROR -1

using namespace vmath;

jint g_width;
jint g_height;
jfloat g_deg;
jfloat g_x;
jfloat g_y;
jfloat g_z;

struct BmpTexture {
	void* img_pixels;
	int img_w;
	int img_h;
};

GLuint vbo[2], nbo, vao, ubo, ebo;

GLfloat ambient[4] = { 1.0, 1.0, 1.0, 0.3 };
GLfloat light_color[3] = { 0.8, 0.8, 0.8};
GLfloat eye_position[3] = { 0.0, 10.0, 30.0 };
GLfloat sky_color[3] = { 0.7, 0.7, 1.0 };
GLfloat ground_color[3] = { 0.2, 0.2, 0.2 };
GLfloat Ns = 8;
GLfloat attenuation = 0.1;

vector<BmpTexture> textures;
GLuint _texture;
