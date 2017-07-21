#include "ModelLoader.h"

using namespace std;
using namespace vmath;

ModelObject ModelLoader::loadObject(const char* obj) {

	ModelObject model;
	istringstream modelStream(obj);
	istringstream value;
	string line, start, temp[3];

	vec3 t_v, t_vn;
	vec2 t_vt;
	uvec3 t_fv, t_ft, t_fn;

	while (getline(modelStream, line)) {
		if (line.empty())
			continue;
		value.clear();
		value.str(line);
		value >> start;
		const char* _str = start.c_str();
		//顶点
		if (strcmp(_str, "v") == 0) {
			value >> t_v[0] >> t_v[1] >> t_v[2];
			model.v.push_back(t_v);
		} else
		//法向量
		if (strcmp(_str, "vn") == 0) {
			value >> t_vn[0] >> t_vn[1] >> t_vn[2];
			model.vn.push_back(t_vn);
		} else
		//纹理
		if (strcmp(_str, "vt") == 0) {
			value >> t_vt[0] >> t_vt[1];
			model.vt.push_back(t_vt);
		} else
		//索引
		if (strcmp(_str, "f") == 0) {
			if (value.str().find('/') == string::npos) {
				value >> t_fv[0] >> t_fv[1] >> t_fv[2];
				t_fv[0]--;
				t_fv[1]--;
				t_fv[2]--;
				//LOGI("fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				model.fv.push_back(t_fv);
			} else {
				value >> temp[0] >> temp[1] >> temp[2];
				int size = 3;
				char str[64];
				for (int i = 0; i < size; i++) {
					strcpy(str, temp[i].c_str());
					char *s = str, *p;
					p = strsep(&s, "/");
					if (p != NULL) {
						t_fv[i] = atoi(p) - 1;
					}
					p = strsep(&s, "/");
					if (p != NULL) {
						t_ft[i] = atoi(p) - 1;
					}
					p = strsep(&s, "/");
					if (p != NULL) {
						t_fn[i] = atoi(p) - 1;
					}
				//	LOGI("fvtn = %d,%d,%d\n", t_fv[0], t_ft[0], t_fn[0]);
				}
				if (t_fv[0]) {
					model.fv.push_back(t_fv);
				//	LOGI("fv_size = %d\n", model.fv.size());
				}
				if (t_ft[0]) {
					model.ft.push_back(t_ft);
				//	LOGI("ft_size = %d\n", model.ft.size());
				}
				if (t_fn[0]) {
					model.fn.push_back(t_fn);
				//	LOGI("fn_size = %d\n", model.fn.size());
				}
			}
		}
	}
	modelStream.str("");
	value.str("");
	start.clear();
	line.clear();
	return model;
}

void ModelLoader::destroyObject(ModelObject obj) {
	obj.fn.clear();
	vector<uvec3>(obj.fn).swap(obj.fn);
	obj.ft.clear();
	vector<uvec3>(obj.ft).swap(obj.ft);
	obj.fv.clear();
	vector<uvec3>(obj.fv).swap(obj.fv);
	obj.v.clear();
	vector<vec3>(obj.v).swap(obj.v);
	obj.vn.clear();
	vector<vec3>(obj.vn).swap(obj.vn);
	obj.vt.clear();
	vector<vec2>(obj.vt).swap(obj.vt);
}
