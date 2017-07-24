#include "com_mumu_glmodel_GLRender.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <vector>
#include <string.h>
#include <cmath>
#include <GLES3/gl3.h>
#include "include/vmath.h"
#include "ShaderLoader.h"
#include "ModelLoader.h"

#define LOG_TAG "NDK_GLRender"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ERROR -1

jint g_width;
jint g_height;
mat4 g_model_ges = rotate(0.0f, 1.0f, 0.0f, 0.0f);
vec3 v_model_loc(0.0f, 0.0f, 0.0f);
mat4 g_model_loc = translate(v_model_loc);
mat4 g_proj;
vec3 v_camera_loc(0.0f, 0.0f, 50.0f);
mat4 g_camera = lookat(v_camera_loc, v_model_loc, vec3(0.0f, 1.0f, 0.0f));

GLuint vbo[3], nbo, vao, ubo, ebo, h_texture;
std::vector<BmpTexture> g_textures;

GLfloat ambient[4] = { 1.0, 1.0, 1.0, 0.3 };
GLfloat light_color[3] = { 0.8, 0.8, 0.8 };
GLfloat light_position[3] = { 0.0f, 0.0f, 400.0f };
GLfloat eye_position[3] = { 0.0, 10.0, 30.0 };
GLfloat sky_color[3] = { 0.7, 0.7, 1.0 };
GLfloat ground_color[3] = { 0.2, 0.2, 0.2 };
GLfloat Ns = 8;
GLfloat attenuation = 0.1;

int useTexture(BmpTexture);
int initShader(GLint , const char*, const char*);

