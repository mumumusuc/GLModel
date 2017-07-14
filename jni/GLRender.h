#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include "include/vmath.h"
#include "ShaderLoader.h"
#include "ModelLoader.h"

#define TAG "NDK_GLRender"
#define ERROR -1

using namespace vmath;

GLint g_progHandler;
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

GLuint vbo, nbo, vao, ubo;
vector<vec3> out_vertices;
vector<vec2> out_uvs;
vector<vec3> out_normals;
vector<Material> out_mtls;
vector<vec3> out_mtlIndex;

vector<BmpTexture> textures;
GLuint _texture;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT int JNICALL
Java_com_mumu_glmodel_GLRender_createProgram(JNIEnv*, jobject);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_resizeWindow(JNIEnv *, jobject, jint, jint);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_initShader(JNIEnv *, jobject, jstring, jstring);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_loadModel(JNIEnv *, jobject, jint, jstring);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_render(JNIEnv *, jobject);

JNIEXPORT int JNICALL
Java_com_mumu_glmodel_GLRender_loadBitmapTextrue(JNIEnv*, jobject,
		jobjectArray);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_glesClean(JNIEnv *, jobject);

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_glesRotateModel(JNIEnv *, jobject, jfloat,
		jfloat, jfloat, jfloat);

#ifdef __cplusplus
}
#endif
