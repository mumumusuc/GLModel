#include "ModelLoader.h"

using namespace std;
using namespace vmath;

ModelObject load_coordinates(const char* obj) {

	ModelObject model;
	istringstream modelStream(obj);
	istringstream value;
	string line, start, temp[3];

	vec3 t_v, t_vn;
	vec2 t_vt;
	uvec3 t_fv, t_ft, t_fn;

	while (getline(modelStream, line)) {
		if (line.length() < 3)
			continue;
		value.clear();
		value.str(line);
		value >> start;
		const char* _str = start.c_str();
//		LOGI("line = %s\n", line.c_str());
		//顶点
		if (strcmp(_str, "v") == 0) {
			value >> t_v[0] >> t_v[1] >> t_v[2];
			model.v.push_back(t_v);
		} else
		//法向量
		if (strcmp(_str, "vn") == 0) {
			value >> t_vn[0] >> t_vn[1] >> t_vn[2];
//			t_vn[0] = -t_vn[0];
//			t_vn[1] = -t_vn[1];
//			t_vn[2] = -t_vn[2];
			model.vn.push_back(t_vn);
		} else
		//纹理
		if (strcmp(_str, "vt") == 0) {
			value >> t_vt[0] >> t_vt[1];
			//	LOGI("vt = %f,%f\n", t_vt[0], t_vt[1]);
			model.vt.push_back(t_vt);
		} else
		//索引
		if (strcmp(_str, "f") == 0) {
			if (value.str().find('/') == string::npos) {
				value >> t_fv[0] >> t_fv[1] >> t_fv[2];
				t_fv[0]--;
				t_fv[1]--;
				t_fv[2]--;
				//	LOGI("fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				model.fv.push_back(t_fv);
			} else {
				value >> temp[0] >> temp[1] >> temp[2];
				//LOGI("value = %s,%s,%s\n", temp[0].c_str(), temp[1].c_str(), temp[2].c_str());
				int size = 3;
				char str[256];
				bool push_fv = false, push_ft = false, push_fn = false;
				for (int i = 0; i < size; i++) {
					strcpy(str, temp[i].c_str());
					char *s = str, *p;
					p = strsep(&s, "/");
					if (*p != 0) {
						push_fv = true;
						t_fv[i] = atoi(p) - 1;
					}
					p = strsep(&s, "/");
					if (*p != 0) {
						push_ft = true;
						t_ft[i] = atoi(p) - 1;
					}
					p = strsep(&s, "/");
					if (*p != 0) {
						push_fn = true;
						t_fn[i] = atoi(p) - 1;
					}
				}
				if (push_fv) {
					model.fv.push_back(t_fv);
					//		LOGI("push fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				}
				if (push_ft) {
					model.ft.push_back(t_ft);
					//	LOGI("push ft = %d,%d,%d\n", t_ft[0],t_ft[1],t_ft[2]);
				}
				if (push_fn) {
					model.fn.push_back(t_fn);
					//	LOGI("push fn = %d,%d,%d\n", t_fn[0],t_fn[1],t_fn[2]);
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

void build_model(ModelObject& obj, float* c_v, float* c_t, float* c_n,
		bool clear) {
	int size, i, j;
	//重建顶点
	if ((size = obj.fv.size())) {
		for (i = 0; i < size; i++) {
			for (j = 0; j < 3; j++) {
				c_v[9 * i + 3 * j + 0] = obj.v[obj.fv[i][j]][0];
				c_v[9 * i + 3 * j + 1] = obj.v[obj.fv[i][j]][1];
				c_v[9 * i + 3 * j + 2] = obj.v[obj.fv[i][j]][2];
			}
		}
	}
	//重建法线
	if ((size = obj.fn.size())) {
		for (i = 0; i < size; i++) {
			for (j = 0; j < 3; j++) {
				c_n[9 * i + 3 * j + 0] = obj.vn[obj.fn[i][j]][0];
				c_n[9 * i + 3 * j + 1] = obj.vn[obj.fn[i][j]][1];
				c_n[9 * i + 3 * j + 2] = obj.vn[obj.fn[i][j]][2];
			}
		}
	}
	//重建纹理
	if ((size = obj.ft.size())) {
		for (i = 0; i < size; i++) {
			for (j = 0; j < 3; j++) {
				c_t[6 * i + 2 * j + 0] = obj.vt[obj.ft[i][j]][0];
				c_t[6 * i + 2 * j + 1] = obj.vt[obj.ft[i][j]][1];
			}
		}
	}
	//释放
	if (clear) {
		destroy_cache(obj);
	}
}

void destroy_cache(ModelObject& obj) {
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
