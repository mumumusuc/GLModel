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

using namespace std;
using namespace vmath;


#ifndef MATERIAL
#define MATERIAL
//材质
struct Material {
	std::string::size_type i;
	unsigned int code;
	float Ns;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	int mtl_index;
};
#endif

#ifndef MODEL_OBJECT
#define MODEL_OBJECT
struct ModelObject {
	//顶点
	vector<vec3> v;
	//顶点索引
	vector<uvec3> fv;
	//法向量
	vector<vec3> vn;
	//法向量索引
	vector<uvec3> fn;
	//材质
	vector<vec2> vt;
	//材质索引
	vector<uvec3> ft;
};
#endif

ModelObject load_coordinates(const char*);
void destroy(ModelObject&);
void load_material(const char*);
void build_model(ModelObject &, float*, float*, float*,bool);

