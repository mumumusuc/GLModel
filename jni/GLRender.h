#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <vector>
#include <string.h>
#include <cmath>
#include <GLES3/gl3.h>
#include "include/vmath.h"

#ifndef GL_RENDER_H
#define GL_RENDER_H

using namespace vmath;

#define LOG_TAG "NDK_GLRender"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define ERROR -1

//纹理
#ifndef BMP_TEXTURE
#define BMP_TEXTURE
struct BmpTexture {
	void* pixels;
	int w;
	int h;
};
#endif

int createProgram(const char*, const char*);
GLuint loadShader(GLenum, const char*);
void resizeWindow(uint, uint);
void bindBuffers(GLfloat*, uint, GLfloat*, uint, GLfloat*, uint, GLuint*);
GLuint bindTexture(void*, uint, uint, GLuint);
void render(GLuint, uint, GLuint, GLuint, GLuint);
void rotateModel(float, float, float, float, float, float);
int initShader(GLint, const char*, const char*);
void clean();

#endif

