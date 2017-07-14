#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include <sstream>
#include "include/vmath.h"

#define TAG "NDK_ModelLoader"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

using namespace std;
using namespace vmath;

//..材质结构体
struct Material {
	string::size_type i;
	unsigned int code;
	float Ns;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	int mtl_index;
};

void inline __clearMaterial__(Material &mtl) {
	mtl.mtl_index = -1;
}

vector<string> split(string, string);

char* readModelSrcFile(char*, AAssetManager*);

bool loadMtl(const char*, vector<Material>&);

bool loadOBJ(const char*, vector<vec3> &, vector<vec2> &, vector<vec3> &,
		vector<vec3> &);
