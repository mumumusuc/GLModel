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
	char* newmtl;
	float Ns;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 Ke;
	float Ni;
	char* name;
};
#endif

#ifndef MODEL_OBJECT
#define MODEL_OBJECT
struct ModelObject {
	char* usemtl;
	//顶点
	float* v;
	uint v_size;
	//法向量
	float* vn;
	uint n_size;
	//材质
	float* vt;
	uint t_size;
};
struct FaceCache {
	char* usemtl;
	vector<uvec3>* fv;
	vector<uvec3>* ft;
	vector<uvec3>* fn;
};
#endif

vector<ModelObject>* load_coordinates(const char*);
vector<Material>* load_mtls(const char*);
void build_vertex(vector<vec3>*, FaceCache*,ModelObject*);
void build_texture(vector<vec2>*, FaceCache*,ModelObject*);
void build_normal(vector<vec3>*, FaceCache*,ModelObject*);
void build_model(ModelObject &, int, float**);

#endif
