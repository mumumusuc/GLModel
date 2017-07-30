#ifndef _H_MODEL_LOADER_
#define _H_MODEL_LOADER_

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <android/log.h>
#include "include/vmath.h"

#define TAG "NDK_ModelLoader"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define BUILD_VERTEX 0
#define BUILD_TEXTURE 1
#define BUILD_NORMAL 2

using namespace std;
using namespace vmath;

#ifndef MATERIAL
#define MATERIAL
//材质
struct Material {
	string newmtl;
	float Ns;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Ni;
	string name;
};
#endif

#ifndef MODEL_OBJECT
#define MODEL_OBJECT
struct ModelObject {
	string usemtl;
	//顶点
	float* v = NULL;
	uint v_size;
	//法向量
	float* vn = NULL;
	uint n_size;
	//材质
	float* vt = NULL;
	uint t_size;
};
struct FaceCache {
	string usemtl;
	vector<ivec3> fv;
	vector<ivec3> ft;
	vector<ivec3> fn;
};
#endif

int load_coordinates(const char*,vector<ModelObject>&);
int load_mtls(const char*,vector<Material>&);
void build_vertex(vector<vec3>, FaceCache,ModelObject&);
void build_texture(vector<vec2>, FaceCache,ModelObject&);
void build_normal(vector<vec3>, FaceCache,ModelObject&);

#endif
