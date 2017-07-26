#include "ModelLoader.h"

using namespace std;
using namespace vmath;

vector<ModelObject>* load_coordinates(const char* obj) {
	if (!obj) {
		LOGE("load_coordinates -> bad input args");
		return NULL;
	}
	istringstream modelStream(obj);
	istringstream value;
	string line, start, temp[3];

	vector<vec3>* v = new vector<vec3>();
	vector<vec2>* vt = new vector<vec2>();
	vector<vec3>* vn = new vector<vec3>();

	vector<FaceCache>* faces = new vector<FaceCache>();
	FaceCache* t_face = new FaceCache();

	vec3 t_v, t_vn;
	vec2 t_vt;
	uvec3 t_fv, t_ft, t_fn;

	LOGE("load_coordinates -> read file now");
	while (getline(modelStream, line)) {
		if (line.length() < 3)
			continue;
		value.clear();
		value.str(line);
		value >> start;
		const char* _str = start.c_str();
		LOGI("line = %s\n", line.c_str());
		//顶点
		if (strcmp(_str, "v") == 0) {
			value >> t_v[0] >> t_v[1] >> t_v[2];
			v->push_back(t_v);
		} else
		//法向量
		if (strcmp(_str, "vn") == 0) {
			value >> t_vn[0] >> t_vn[1] >> t_vn[2];
			vn->push_back(t_vn);
		} else
		//纹理
		if (strcmp(_str, "vt") == 0) {
			value >> t_vt[0] >> t_vt[1];
			LOGI("vt = %f,%f\n", t_vt[0], t_vt[1]);
			vt->push_back(t_vt);
		} else
		//材质
		if (strcmp(_str, "usemtl") == 0) {
			value >> t_face->usemtl;
			LOGI("usemtl %s", t_face->usemtl);
		} else
		//索引
		if (strcmp(_str, "f") == 0) {
			if (value.str().find('/') == string::npos) {
				value >> t_fv[0] >> t_fv[1] >> t_fv[2];
				t_fv[0]--;
				t_fv[1]--;
				t_fv[2]--;
				LOGI("fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);
				t_face->fv->push_back(t_fv);
			} else {
				value >> temp[0] >> temp[1] >> temp[2];
				LOGI("value = %s,%s,%s\n", temp[0].c_str(), temp[1].c_str(),
						temp[2].c_str());
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
					if(t_face->fv != NULL){
					t_face->fv->push_back(t_fv);
					LOGI("push fv = %d,%d,%d\n", t_fv[0], t_fv[1], t_fv[2]);}else{
						LOGE("push fv = NULL");
					}
				}
				if (push_ft) {
					t_face->ft->push_back(t_ft);
					LOGI("push ft = %d,%d,%d\n", t_ft[0], t_ft[1], t_ft[2]);
				}
				if (push_fn) {
					t_face->fn->push_back(t_fn);
					LOGI("push fn = %d,%d,%d\n", t_fn[0], t_fn[1], t_fn[2]);
				}
			}
		}
	}
	modelStream.str("");
	value.str("");
	start.clear();
	line.clear();
	LOGE("load_coordinates -> read file end");
	LOGE("load_coordinates -> build indice now");
	vector<ModelObject>* models = new vector<ModelObject>();
	ModelObject* model = new ModelObject();
	for (int i = 0, size = faces->size(); i < size; i++) {
		model->usemtl = (*faces)[i].usemtl;
		LOGI("model[%d].usemtl = %s", i, model->usemtl);
		model->v = build_vertex(v, &(*faces)[i]);
		LOGI("model[%d].v = %d", i, model->v);
		model->vt = build_texture(vt, &(*faces)[i]);
		LOGI("model[%d].vt = %d", i, model->vt);
		model->vn = build_normal(vn, &(*faces)[i]);
		LOGI("model[%d].vn = %d", i, model->vn);
	}
	LOGE("load_coordinates -> build indice end");
	delete model;
	model = NULL;
	delete v;
	v = NULL;
	delete vt;
	vt = NULL;
	delete vn;
	vn = NULL;
	delete faces;
	faces = NULL;
	delete t_face;
	t_face = NULL;
	return models;
}

vector<Material>* load_mtls(const char* mtl) {
	if (!mtl) {
		LOGE("load_mtls -> bad input args");
		return NULL;
	}
	istringstream modelStream(mtl);
	istringstream value;
	string line, start, temp[3];
	vector<Material>* v_material = new vector<Material>();
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
		} else if (strcmp(_str, "Ke") == 0) {
			value >> t_material.Ke[0] >> t_material.Ke[1] >> t_material.Ke[2];
		} else if (strcmp(_str, "Ni") == 0) {
			value >> t_material.Ni;
		} else if (strcmp(_str, "map_Kd") == 0) {
			value >> t_material.name;
			v_material->push_back(t_material);
		}
	}
	modelStream.str("");
	value.str("");
	start.clear();
	line.clear();
	return v_material;
}

float* build_vertex(vector<vec3>* points, FaceCache* face) {
	float* p = NULL;
	int size = face->fv->size();
	if (size) {
		p = (float*) malloc(size * 3 * 3 * sizeof(float));
		LOGI("build_vertex -> load_vertex_p = %d", p);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				p[9 * i + 3 * j + 0] = (*points)[*(face->fv)[i][j]][0];
				p[9 * i + 3 * j + 1] = (*points)[*(face->fv)[i][j]][1];
				p[9 * i + 3 * j + 2] = (*points)[*(face->fv)[i][j]][2];
			}
		}
	}
	return p;
}

float* build_texture(vector<vec2>* points, FaceCache* face) {
	float* p = NULL;
	int size = face->ft->size();
	if (size) {
		p = (float*) malloc(size * 3 * 2 * sizeof(float));
		LOGI("build_texture -> load_texture_p = %d", p);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				p[6 * i + 2 * j + 0] = (*points)[*(face->ft)[i][j]][0];
				p[6 * i + 2 * j + 1] = (*points)[*(face->ft)[i][j]][1];
			}
		}
	}
	return p;
}
float* build_normal(vector<vec3>* points, FaceCache* face) {
	float* p = NULL;
	int size = face->fv->size();
	if (size) {
		p = (float*) malloc(size * 3 * 3 * sizeof(float));
		LOGI("build_normal -> load_normal_p = %d", p);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < 3; j++) {
				p[9 * i + 3 * j + 0] = (*points)[*(face->fn)[i][j]][0];
				p[9 * i + 3 * j + 1] = (*points)[*(face->fn)[i][j]][1];
				p[9 * i + 3 * j + 2] = (*points)[*(face->fn)[i][j]][2];
			}
		}
	}
	return p;
}

