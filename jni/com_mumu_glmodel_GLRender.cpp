#include "GLRender.h"
#include "ModelLoader.h"

JNIEXPORT jobjectArray JNICALL Java_com_mumu_glmodel_GLRender_loadModel(
		JNIEnv *env, jobject obj, jstring model, jstring mtl) {
	if (!model) {
		LOGE("loadModel -> bad input args");
		return NULL;
	}
	jclass _c_ModelStruct = env->FindClass("com/mumu/glmodel/ModelStruct");
	if (_c_ModelStruct == 0)
		LOGE("error on find class");
	jmethodID _m_init = env->GetMethodID(_c_ModelStruct, "<init>", "()V");
	if (_m_init == 0)
		LOGE("error on get constructor");
	//解析文本
	const char* _model = env->GetStringUTFChars(model, 0);
	LOGE("loadModel -> parse coordinates now");
	vector<ModelObject> objects;
	int model_size = load_coordinates(_model, objects);
	LOGE("loadModel -> parse coordinates end");
	env->ReleaseStringUTFChars(model, _model);
	vector<Material> materials;
	int mtl_size = 0;
	if (mtl) {
		const char* _mtl = env->GetStringUTFChars(mtl, 0);
		LOGE("loadModel -> parse material now");
		mtl_size = load_mtls(_mtl, materials);
		LOGE("loadModel -> parse material end");
		env->ReleaseStringUTFChars(mtl, _mtl);
	}
	LOGE("loadModel -> get [%d,%d] objects", model_size, mtl_size);
	if (!model_size) {
		return NULL;
	}
	jobjectArray _result = env->NewObjectArray(model_size, _c_ModelStruct,
	NULL);
	for (int i = 0; i < model_size; i++) {
		jobject _o_instance = env->NewObject(_c_ModelStruct, _m_init);
		if (_o_instance == 0)
			LOGE("error on new instance");
		ModelObject object = objects[i];
		//重建索引
		const uint v_size = object.v_size;
		const uint t_size = object.t_size;
		const uint n_size = object.n_size;
		LOGI("loadModel -> size = [%d,%d,%d]", v_size, t_size, n_size);
		//顶点
		if (v_size) {
			jfieldID _f_mVertexSize = env->GetFieldID(_c_ModelStruct,
					"mVertexSize", "I");
			if (_f_mVertexSize == 0)
				LOGE("error on get mVertexSize");
			env->SetIntField(_o_instance, _f_mVertexSize, v_size);
			jfieldID _f_mVertexCoBuffer = env->GetFieldID(_c_ModelStruct,
					"mVertexCoBuffer", "[F");
			if (_f_mVertexCoBuffer == 0)
				LOGE("error on get mVertexCoBuffer");
			jfloatArray _vertex_buffer = env->NewFloatArray(v_size);
			env->SetFloatArrayRegion(_vertex_buffer, 0, v_size, object.v);
			env->SetObjectField(_o_instance, _f_mVertexCoBuffer,
					_vertex_buffer);
			env->DeleteLocalRef(_vertex_buffer);
			free(object.v);
			object.v = NULL;
			LOGI("loadModel -> for v freedom !!!");
		}
		//纹理
		if (t_size) {
			jfieldID _f_mTextureSize = env->GetFieldID(_c_ModelStruct,
					"mTextureSize", "I");
			if (_f_mTextureSize == 0)
				LOGE("error on get mTextureSize");
			env->SetIntField(_o_instance, _f_mTextureSize, t_size);
			jfieldID _f_mTextureCoBuffer = env->GetFieldID(_c_ModelStruct,
					"mTextureCoBuffer", "[F");
			if (_f_mTextureCoBuffer == 0)
				LOGE("error on get mTextureCoBuffer");
			jfloatArray _texture_buffer = env->NewFloatArray(t_size);
			env->SetFloatArrayRegion(_texture_buffer, 0, t_size, object.vt);
			env->SetObjectField(_o_instance, _f_mTextureCoBuffer,
					_texture_buffer);
			env->DeleteLocalRef(_texture_buffer);
			free(object.vt);
			object.vt = NULL;
			LOGI("loadModel -> for vt freedom !!!");
		}
		//法线
		if (n_size) {
			jfieldID _f_mNormalSize = env->GetFieldID(_c_ModelStruct,
					"mNormalSize", "I");
			if (_f_mNormalSize == 0)
				LOGE("error on get mNormalSize");
			env->SetIntField(_o_instance, _f_mNormalSize, n_size);
			jfieldID _f_mNormalCoBuffer = env->GetFieldID(_c_ModelStruct,
					"mNormalCoBuffer", "[F");
			if (_f_mNormalCoBuffer == 0)
				LOGE("error on get mNormalCoBuffer");
			jfloatArray _normal_buffer = env->NewFloatArray(n_size);
			env->SetFloatArrayRegion(_normal_buffer, 0, n_size, object.vn);
			env->SetObjectField(_o_instance, _f_mNormalCoBuffer,
					_normal_buffer);
			env->DeleteLocalRef(_normal_buffer);
			free(object.vn);
			object.vn = NULL;
			LOGI("loadModel -> for vn freedom !!!");
		}
		if (mtl_size) {
			jfieldID _f_mUseMtl = env->GetFieldID(_c_ModelStruct, "mUseMtl",
					"Ljava/lang/String;");
			if (_f_mUseMtl == 0)
				LOGE("error on get  mUseMtl");
			jfieldID _f_Ns = env->GetFieldID(_c_ModelStruct, "Ns", "F");
			if (_f_Ns == 0)
				LOGE("error on get  Ns");
			jfieldID _f_Ka = env->GetFieldID(_c_ModelStruct, "Ka", "[F");
			if (_f_Ka == 0)
				LOGE("error on get  Ka");
			jfieldID _f_Kd = env->GetFieldID(_c_ModelStruct, "Kd", "[F");
			if (_f_Kd == 0)
				LOGE("error on get  Kd");
			jfieldID _f_Ks = env->GetFieldID(_c_ModelStruct, "Ks", "[F");
			if (_f_Ks == 0)
				LOGE("error on get  Ks");
			jfieldID _f_Ke = env->GetFieldID(_c_ModelStruct, "Ke", "[F");
			if (_f_Ke == 0)
				LOGE("error on get  Ke");
			jfieldID _f_Ni = env->GetFieldID(_c_ModelStruct, "Ni", "F");
			if (_f_Ni == 0)
				LOGE("error on get  Ni");
			//材质
			for (int j = 0; j < mtl_size; j++) {
				Material material = materials[j];
				string new_mtl = material.newmtl;
				if (strcmp(object.usemtl.c_str(), new_mtl.c_str()) == 0) {
					LOGI("loadModel -> bind mtl_name = %s", new_mtl.c_str());
					jstring _mUseMtl = env->NewStringUTF(material.name.c_str());
					env->SetObjectField(_o_instance, _f_mUseMtl, _mUseMtl);
					env->ReleaseStringUTFChars(_mUseMtl, material.name.c_str());
					env->DeleteLocalRef(_mUseMtl);
					env->SetFloatField(_o_instance, _f_Ns, material.Ns);
					env->SetFloatField(_o_instance, _f_Ni, material.Ni);
					jfloatArray _Ka = env->NewFloatArray(3);
					env->SetFloatArrayRegion(_Ka, 0, 3, &material.Ka[0]);
					env->SetObjectField(_o_instance, _f_Ka, _Ka);
					//	env->ReleaseFloatArrayElements(_Ka, &material.Ka[0], 0);
					env->DeleteLocalRef(_Ka);
					jfloatArray _Kd = env->NewFloatArray(3);
					env->SetFloatArrayRegion(_Kd, 0, 3, &material.Kd[0]);
					env->SetObjectField(_o_instance, _f_Kd, _Kd);
					//	env->ReleaseFloatArrayElements(_Kd, &material.Kd[0], 0);
					env->DeleteLocalRef(_Kd);
					jfloatArray _Ks = env->NewFloatArray(3);
					env->SetFloatArrayRegion(_Ks, 0, 3, &material.Ks[0]);
					env->SetObjectField(_o_instance, _f_Ks, _Ks);
					//	env->ReleaseFloatArrayElements(_Ks, &material.Ks[0], 0);
					env->DeleteLocalRef(_Ks);
					jfloatArray _Ke = env->NewFloatArray(3);
					env->SetFloatArrayRegion(_Ke, 0, 3, &material.Ke[0]);
					env->SetObjectField(_o_instance, _f_Ke, _Ke);
					//	env->ReleaseFloatArrayElements(_Ke, &material.Ke[0], 0);
					env->DeleteLocalRef(_Ke);
				}
			}
		}
		//
		env->SetObjectArrayElement(_result, i, _o_instance);
		env->DeleteLocalRef(_o_instance);
	}
	return _result;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_genRenderParams(
		JNIEnv *env, jobject obj, jobject model) {
	if (!model) {
		LOGE("genRenderParams -> bad model arg");
		return;
	}
	jclass _c_ModelStruct = env->GetObjectClass(model);
	jfieldID _f_mVertexSize = env->GetFieldID(_c_ModelStruct, "mVertexSize",
			"I");
	if (_f_mVertexSize == 0)
		LOGE("error on get mVertexSize");
	jint v_size = env->GetIntField(model, _f_mVertexSize);
	jfloat* _v = NULL;
	jfloatArray v = NULL;
	jfieldID _f_mVertexCoBuffer = env->GetFieldID(_c_ModelStruct,
			"mVertexCoBuffer", "[F");
	if (_f_mVertexCoBuffer == 0)
		LOGE("error on get mVertexCoBuffer");
	if (v_size) {
		v = (jfloatArray) env->GetObjectField(model, _f_mVertexCoBuffer);
		_v = env->GetFloatArrayElements(v, 0);
	}
	//
	jfieldID _f_mTextureSize = env->GetFieldID(_c_ModelStruct, "mTextureSize",
			"I");
	if (_f_mTextureSize == 0)
		LOGE("error on get mTextureSize");
	jint t_size = env->GetIntField(model, _f_mTextureSize);
	jfloat* _t = NULL;
	jfloatArray t = NULL;
	jfieldID _f_mTextureCoBuffer = env->GetFieldID(_c_ModelStruct,
			"mTextureCoBuffer", "[F");
	if (_f_mTextureCoBuffer == 0)
		LOGE("error on get mTextureCoBuffer");
	if (t_size) {
		t = (jfloatArray) env->GetObjectField(model, _f_mTextureCoBuffer);
		_t = env->GetFloatArrayElements(t, 0);
	}
	//
	jfieldID _f_mNormalSize = env->GetFieldID(_c_ModelStruct, "mNormalSize",
			"I");
	if (_f_mNormalSize == 0)
		LOGE("error on get mNormalSize");
	jint n_size = env->GetIntField(model, _f_mNormalSize);
	jfloat* _n = NULL;
	jfloatArray n = NULL;
	jfieldID _f_mNormalCoBuffer = _f_mNormalCoBuffer = env->GetFieldID(
			_c_ModelStruct, "mNormalCoBuffer", "[F");
	if (_f_mNormalCoBuffer == 0)
		LOGE("error on get mNormalCoBuffer");
	if (n_size) {

		n = (jfloatArray) env->GetObjectField(model, _f_mNormalCoBuffer);
		_n = env->GetFloatArrayElements(n, 0);
	}
	//绑定到GPU
	GLuint _handle[4];
	bindBuffers(_v, v_size, _t, t_size, _n, n_size, _handle);
	LOGI("_handle = [%d,%d,%d,%d]", _handle[0], _handle[1], _handle[2],
			_handle[3]);

	jfieldID _f_H_VAO = env->GetFieldID(_c_ModelStruct, "_H_VAO", "I");
	if (_f_H_VAO == 0)
		LOGE("error on get _H_VAO");
	env->SetIntField(model, _f_H_VAO, _handle[0]);

	jfieldID _f_H_vertex = env->GetFieldID(_c_ModelStruct, "_H_vertex_co", "I");
	if (_f_H_vertex == 0)
		LOGE("error on get _H_vertex_co");
	env->SetIntField(model, _f_H_vertex, _handle[1]);

	jfieldID _f_H_texture = env->GetFieldID(_c_ModelStruct, "_H_texture_co",
			"I");
	if (_f_H_texture == 0)
		LOGE("error on get _H_texture_co");
	env->SetIntField(model, _f_H_texture, _handle[2]);

	jfieldID _f_H_normal = env->GetFieldID(_c_ModelStruct, "_H_normal_co", "I");
	if (_f_H_normal == 0)
		LOGE("error on get _H_normal_co");
	env->SetIntField(model, _f_H_normal, _handle[3]);
	if (v && _v) {
		env->ReleaseFloatArrayElements(v, _v, 0);
		env->DeleteLocalRef(v);
	}
	if (t && _t) {
		env->ReleaseFloatArrayElements(t, _t, 0);
		env->DeleteLocalRef(t);
	}
	if (n && _n) {
		env->ReleaseFloatArrayElements(n, _n, 0);
		env->DeleteLocalRef(n);
	}
	env->SetObjectField(model, _f_mVertexCoBuffer, NULL);
	env->SetObjectField(model, _f_mTextureCoBuffer, NULL);
	env->SetObjectField(model, _f_mNormalCoBuffer, NULL);
}

