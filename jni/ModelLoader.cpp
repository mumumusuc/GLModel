#include "include/ModelLoader.h"

using namespace std;
using namespace vmath;
int load_coordinates(const char* obj, vector<ModelObject>& models) {
	if (!obj) {
		LOGE("load_coordinates -> bad input args");
		return NULL;
	}
	istringstream modelStream(obj);
	istringstream value;
	string line, start, temp[3];
	vector<vec3> v;
	vector<vec2> vt;
	vector<vec3> vn;
	vector<FaceCache> faces;
	FaceCache t_face;

	vec3 t_v, t_vn;
	vec2 t_vt;
	ivec3 t_fv, t_ft, t_fn;
	bool push_fv = 0, push_ft = 0, push_fn = 0;

	LOGE("load_coordinates -> read file now");
	while (getline(modelStream, line)) {
		if (line.length() < 3)
			continue;
		value.clear();
		value.str(line);
		value >> start;
		//LOGI("line = %s\n", line.c_str());
		//顶点
		if (strcmp(start.c_str(), "v") == 0) {
			value >> t_v[0] >> t_v[1] >> t_v[2];
			v.push_back(t_v);
		} else
		//法向量
		if (strcmp(start.c_str(), "vn") == 0) {
			value >> t_vn[0] >> t_vn[1] >> t_vn[2];
			vn.push_back(t_vn);
		} else
		//纹理
		if (strcmp(start.c_str(), "vt") == 0) {
			value >> t_vt[0] >> t_vt[1];
			t_vt[1] = -t_vt[1];
			vt.push_back(t_vt);
		} else
		//材质
		if (strcmp(start.c_str(), "usemtl") == 0) {
			if (!t_face.usemtl.empty()) {
				faces.push_back(t_face);
				t_face.fv.clear();
				vector<ivec3>(t_face.fv).swap(t_face.fv);
				t_face.ft.clear();
				vector<ivec3>(t_face.ft).swap(t_face.ft);
				t_face.fn.clear();
				vector<ivec3>(t_face.fn).swap(t_face.fn);
			}
			value >> t_face.usemtl;
			LOGI("usemtl %s", t_face.usemtl.c_str());
		} else
		//索引
		if (strcmp(start.c_str(), "f") == 0) {
			if (value.str().find('/') == string::npos) {
				value >> t_fv[0] >> t_fv[1] >> t_fv[2];
				t_fv[0]--;
				t_fv[1]--;
				t_fv[2]--;
				//LOGI("fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				t_face.fv.push_back(t_fv);
			} else {
				value >> temp[0] >> temp[1] >> temp[2];
//				LOGI("value = %s,%s,%s\n", temp[0].c_str(), temp[1].c_str(),temp[2].c_str());
				int size = 3;
				char str[256];
				for (int i = 0; i < size; i++) {
					strcpy(str, temp[i].c_str());
					char *s = str, *p;
					p = strsep(&s, "/");
					if (*p != 0) {
						t_fv[i] = atoi(p) - 1;
						push_fv = true;
					}
					p = strsep(&s, "/");
					if (*p != 0) {
						t_ft[i] = atoi(p) - 1;
						push_ft = true;
					}
					p = strsep(&s, "/");
					if (*p != 0) {
						t_fn[i] = atoi(p) - 1;
						push_fn = true;
					}
				}
				if (push_fv) {
					t_face.fv.push_back(t_fv);
//					LOGI("push fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				}
				if (push_ft) {
					t_face.ft.push_back(t_ft);
//					LOGI("push ft = %d,%d,%d\n", t_ft[0], t_ft[1], t_ft[2]);
				}
				if (push_fn) {
					t_face.fn.push_back(t_fn);
//					LOGI("push fn = %d,%d,%d\n", t_fn[0], t_fn[1], t_fn[2]);
				}
				push_fv = push_ft = push_fn = false;
			}
		}
	}
	faces.push_back(t_face);

	modelStream.str("");
	value.str("");
	start.clear();
	line.clear();
	temp[0].clear();
	temp[1].clear();
	temp[2].clear();

	LOGE("load_coordinates -> read file end");
	LOGE("load_coordinates -> build indice now");

	for (int i = 0, size = faces.size(); i < size; i++) {
		ModelObject* model = new ModelObject();
		FaceCache face = faces[i];
		model->usemtl = face.usemtl;
		LOGI("model[%d].usemtl = %s", i, model->usemtl.c_str());
		build_vertex(v, face, *model);
		face.fv.clear();
		vector<ivec3>(face.fv).swap(face.fv);
		build_texture(vt, face, *model);
		face.ft.clear();
		vector<ivec3>(face.ft).swap(face.ft);
		build_normal(vn, face, *model);
		face.fn.clear();
		vector<ivec3>(face.fn).swap(face.fn);
		models.push_back(*model);
	}
	faces.clear();
	vector<FaceCache>(faces).swap(faces);
	LOGE("load_coordinates -> build indice end");
	v.clear();
	vector<vec3>(v).swap(v);
	vt.clear();
	vector<vec2>(vt).swap(vt);
	vn.clear();
	vector<vec3>(vn).swap(vn);
	LOGI("load_coordinates -> get %d objects", models.size());
	return models.size();
}

int load_mtls(const char* mtl, vector<Material>& v_material) {
	if (!mtl) {
		LOGE("load_mtls -> bad input args");
		return NULL;
	}
	istringstream modelStream(mtl);
	istringstream value;
	string line, start, temp[3];
	Material t_material;
	while (getline(modelStream, line)) {
		if (line.length() < 3)
			continue;
		value.clear();
		value.str(line);
		value >> start;
		const char* _str = start.c_str();
		//LOGI("line = %s\n", line.c_str());
		if (strcmp(_str, "newmtl") == 0) {
			value >> t_material.newmtl;
		} else if (strcmp(_str, "Ns") == 0) {
			value >> t_material.Ns;
		} else if (strcmp(_str, "Ka") == 0) {
			value >> t_material.Ka[0] >> t_material.Ka[1] >> t_material.Ka[2];
		} else if (strcmp(_str, "Kd") == 0) {
			value >> t_material.Kd[0] >> t_material.Kd[1] >> t_material.Kd[2];
		} else if (strcmp(_str, "Ks") == 0) {
			value >> t_material.Ks[0] >> t_material.Ks[1] >> t_material.Ks[2];
		} else if (strcmp(_str, "Ni") == 0) {
			value >> t_material.Ni;
		} else if (strcmp(_str, "map_Kd") == 0) {
			value >> t_material.name;
			v_material.push_back(t_material);
		}
	}
	modelStream.str("");
	value.str("");
	start.clear();
	line.clear();
	LOGI("load_mtls -> get %d objects", v_material.size());
	return v_material.size();
}

void build_vertex(vector<vec3> points, FaceCache face, ModelObject& obj) {
	if (obj.v != NULL) {
		LOGE("build_vertex -> release v first, v = %d", obj.v);
		free(obj.v);
		obj.v = NULL;
	}
	float* p = NULL;
	int size = face.fv.size();
	obj.v_size = size * 9;
	LOGI("build_vertex -> load_vertex, size = %d", obj.v_size);
	if (size) {
		p = (float*) malloc(obj.v_size * sizeof(float));
		for (int i = 0; i < size; i++) {
			//LOGI("build_vertex -> [%d,%d,%d]", face.fv[i][0], face.fv[i][1],face.fv[i][2]);
			for (int j = 0; j < 3; j++) {
				p[9 * i + 3 * j + 0] = points[face.fv[i][j]][0];
				p[9 * i + 3 * j + 1] = points[face.fv[i][j]][1];
				p[9 * i + 3 * j + 2] = points[face.fv[i][j]][2];
			}
		}
	}
	LOGI("build_vertex -> load_vertex end");
	obj.v = p;
}

void build_texture(vector<vec2> points, FaceCache face, ModelObject& obj) {
	if (obj.vt != NULL) {
		LOGE("build_texture -> release vt first");
		free(obj.vt);
		obj.vt = NULL;
	}
	float* p = NULL;
	int size = face.ft.size();
	obj.t_size = size * 6;
	LOGI("build_vertex -> build_texture,size = %d", obj.t_size);
	if (size) {
		p = (float*) malloc(obj.t_size * sizeof(float));
		for (int i = 0; i < size; i++) {
			//LOGI("build_texture -> [%d,%d]", face.ft[i][0], face.ft[i][0]);
			for (int j = 0; j < 3; j++) {
				p[6 * i + 2 * j + 0] = points[face.ft[i][j]][0];
				p[6 * i + 2 * j + 1] = points[face.ft[i][j]][1];
			}
		}
	}
	LOGI("build_vertex -> build_texture end");
	obj.vt = p;
}
void build_normal(vector<vec3> points, FaceCache face, ModelObject& obj) {
	if (obj.vn != NULL) {
		LOGE("build_normal -> release vn first");
		free(obj.vn);
		obj.vn = NULL;
	}
	float* p = NULL;
	int size = face.fn.size();
	obj.n_size = size * 9;
	LOGI("build_vertex -> build_normal, size = %d", obj.n_size);
	if (size) {
		p = (float*) malloc(obj.n_size * sizeof(float));
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				//LOGI("build_normal -> [%d]", (*(face->fn))[i][j]);
				p[9 * i + 3 * j + 0] = points[face.fn[i][j]][0];
				p[9 * i + 3 * j + 1] = points[face.fn[i][j]][1];
				p[9 * i + 3 * j + 2] = points[face.fn[i][j]][2];
			}
		}
	}
	LOGI("build_vertex -> build_normal end");
	obj.vn = p;
}

