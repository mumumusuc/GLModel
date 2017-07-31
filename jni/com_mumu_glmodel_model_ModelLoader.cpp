#include "include/com_mumu_glmodel_model_ModelLoader.h"
#include "include/ModelLoader.h"

#define TAG "NDK_ModelLoader"

JNIEXPORT jobjectArray JNICALL Java_com_mumu_glmodel_model_ModelLoader_parseModelCoordinate(
		JNIEnv *env, jclass clz, jstring model) {
	if (!model) {
		LOGE("load_model_coordinate -> bad input args");
		return NULL;
	}
	const char* _model = env->GetStringUTFChars(model, 0);
	LOGE("load_model_coordinate -> parse  now");
	vector<ModelObject> objects;
	int model_size = load_coordinates(_model, objects);
	LOGE("load_model_coordinate -> parse  end, %d got", model_size);
	env->ReleaseStringUTFChars(model, _model);
	if (!model_size)
		return NULL;

	jclass _c_ModelCoordinate = env->FindClass(
			"com/mumu/glmodel/model/ModelCoordinate");
	if (_c_ModelCoordinate == 0)
		LOGE("error on find class");
	jmethodID _m_init = env->GetMethodID(_c_ModelCoordinate, "<init>", "()V");
	if (_m_init == 0)
		LOGE("error on get constructor");
	jobjectArray _o_coordinates = env->NewObjectArray(model_size,
			_c_ModelCoordinate, NULL);
	jfieldID _f_mVertexSize = env->GetFieldID(_c_ModelCoordinate, "mVertexSize",
			"I");
	if (_f_mVertexSize == 0)
		LOGE("error on get mVertexSize");
	jfieldID _f_mVertexCoBuffer = env->GetFieldID(_c_ModelCoordinate,
			"mVertexCoBuffer", "[F");
	if (_f_mVertexCoBuffer == 0)
		LOGE("error on get mVertexCoBuffer");
	jfieldID _f_mTextureSize = env->GetFieldID(_c_ModelCoordinate,
			"mTextureSize", "I");
	if (_f_mTextureSize == 0)
		LOGE("error on get mTextureSize");
	jfieldID _f_mTextureCoBuffer = env->GetFieldID(_c_ModelCoordinate,
			"mTextureCoBuffer", "[F");
	if (_f_mTextureCoBuffer == 0)
		LOGE("error on get mTextureCoBuffer");
	jfieldID _f_mNormalSize = env->GetFieldID(_c_ModelCoordinate, "mNormalSize",
			"I");
	if (_f_mNormalSize == 0)
		LOGE("error on get mNormalSize");
	jfieldID _f_mNormalCoBuffer = env->GetFieldID(_c_ModelCoordinate,
			"mNormalCoBuffer", "[F");
	if (_f_mNormalCoBuffer == 0)
		LOGE("error on get mNormalCoBuffer");
	LOGE("load_model_coordinate -> rebuild coordinate");
	for (int i = 0; i < model_size; i++) {
		jobject _o_instance = env->NewObject(_c_ModelCoordinate, _m_init);
		ModelObject object = objects[i];

		if (!object.usemtl.empty()) {
			jfieldID _f_mUseMtl = env->GetFieldID(_c_ModelCoordinate, "mUseMtl",
					"Ljava/lang/String;");
			if (_f_mUseMtl == 0)
				LOGE("error on get  mUseMtl");
			jstring _mUseMtl = env->NewStringUTF(object.usemtl.c_str());
			env->SetObjectField(_o_instance, _f_mUseMtl, _mUseMtl);
			env->ReleaseStringUTFChars(_mUseMtl, object.usemtl.c_str());
			env->DeleteLocalRef(_mUseMtl);
		}
		//重建索引
		const uint v_size = object.v_size;
		const uint t_size = object.t_size;
		const uint n_size = object.n_size;
		LOGI("load_model_coordinate -> size = [%d,%d,%d]", v_size, t_size,
				n_size);
		//顶点
		if (v_size) {
		 env->SetIntField(_o_instance, _f_mVertexSize, v_size);
		 jfloatArray _vertex_buffer = env->NewFloatArray(v_size);
		 env->SetFloatArrayRegion(_vertex_buffer, 0, v_size, object.v);
		 env->SetObjectField(_o_instance, _f_mVertexCoBuffer,
		 _vertex_buffer);
		 env->DeleteLocalRef(_vertex_buffer);
			free(object.v);
			object.v = NULL;
			LOGI("load_model_coordinate -> for v freedom !!!");
		}
		//纹理
		if (t_size) {
		 env->SetIntField(_o_instance, _f_mTextureSize, t_size);
		 jfloatArray _texture_buffer = env->NewFloatArray(t_size);
		 env->SetFloatArrayRegion(_texture_buffer, 0, t_size, object.vt);
		 env->SetObjectField(_o_instance, _f_mTextureCoBuffer,
		 _texture_buffer);
		 env->DeleteLocalRef(_texture_buffer);
			free(object.vt);
			object.vt = NULL;
			LOGI("load_model_coordinate -> for vt freedom !!!");
		}
		//法线
		if (n_size) {
		 env->SetIntField(_o_instance, _f_mNormalSize, n_size);
		 jfloatArray _normal_buffer = env->NewFloatArray(n_size);
		 env->SetFloatArrayRegion(_normal_buffer, 0, n_size, object.vn);
		 env->SetObjectField(_o_instance, _f_mNormalCoBuffer,
		 _normal_buffer);
		 env->DeleteLocalRef(_normal_buffer);
			free(object.vn);
			object.vn = NULL;
			LOGI("load_model_coordinate -> for vn freedom !!!");
		}
		env->SetObjectArrayElement(_o_coordinates, i, _o_instance);
		env->DeleteLocalRef(_o_instance);
	}
	objects.clear();
	vector<ModelObject>(objects).swap(objects);
	return _o_coordinates;
	//return NULL;
}

JNIEXPORT jobjectArray JNICALL Java_com_mumu_glmodel_model_ModelLoader_parseModelMtl(
		JNIEnv *env, jclass clz, jstring mtl) {
	if (!mtl) {
		LOGE("load_model_mtl -> bad input args");
		return NULL;
	}
	const char* _mtl = env->GetStringUTFChars(mtl, 0);
	vector<Material> mtls;
	LOGE("load_model_mtl -> parse material now");
	int mtl_size = load_mtls(_mtl, mtls);
	LOGE("load_model_mtl -> parse material end, %d got", mtl_size);
	env->ReleaseStringUTFChars(mtl, _mtl);
	if (!mtl_size) {
		return NULL;
	}
	jclass _c_ModelMtl = env->FindClass("com/mumu/glmodel/model/ModelMtl");
	if (_c_ModelMtl == 0)
		LOGE("error on find class");
	jmethodID _m_init = env->GetMethodID(_c_ModelMtl, "<init>", "()V");
	if (_m_init == 0)
		LOGE("error on get constructor");
	jobjectArray _o_mtls = env->NewObjectArray(mtl_size, _c_ModelMtl, NULL);
	jfieldID _f_mTextureName = env->GetFieldID(_c_ModelMtl, "mTextureName",
			"Ljava/lang/String;");
	if (_f_mTextureName == 0)
		LOGE("error on get  mTextureName");
	jfieldID _f_mNewMtl = env->GetFieldID(_c_ModelMtl, "mNewMtl",
			"Ljava/lang/String;");
	if (_f_mNewMtl == 0)
		LOGE("error on get  mNewMtl");
	jfieldID _f_Ns = env->GetFieldID(_c_ModelMtl, "Ns", "F");
	if (_f_Ns == 0)
		LOGE("error on get  Ns");
	jfieldID _f_Ka = env->GetFieldID(_c_ModelMtl, "Ka", "[F");
	if (_f_Ka == 0)
		LOGE("error on get  Ka");
	jfieldID _f_Kd = env->GetFieldID(_c_ModelMtl, "Kd", "[F");
	if (_f_Kd == 0)
		LOGE("error on get  Kd");
	jfieldID _f_Ks = env->GetFieldID(_c_ModelMtl, "Ks", "[F");
	if (_f_Ks == 0)
		LOGE("error on get  Ks");
	jfieldID _f_Ni = env->GetFieldID(_c_ModelMtl, "Ni", "F");
	if (_f_Ni == 0)
		LOGE("error on get  Ni");

	for (int i = 0; i < mtl_size; i++) {
		jobject _o_instance = env->NewObject(_c_ModelMtl, _m_init);
		Material object = mtls[i];
		jstring _mTextureName = env->NewStringUTF(object.name.c_str());
		env->SetObjectField(_o_instance, _f_mTextureName, _mTextureName);
		env->ReleaseStringUTFChars(_mTextureName, object.name.c_str());
		env->DeleteLocalRef(_mTextureName);
		jstring _mNewMtl = env->NewStringUTF(object.newmtl.c_str());
		env->SetObjectField(_o_instance, _f_mNewMtl, _mNewMtl);
		env->ReleaseStringUTFChars(_mNewMtl, object.name.c_str());
		env->DeleteLocalRef(_mNewMtl);
		env->SetFloatField(_o_instance, _f_Ns, object.Ns);
		env->SetFloatField(_o_instance, _f_Ni, object.Ni);
		jfloatArray _Ka = env->NewFloatArray(3);
		env->SetFloatArrayRegion(_Ka, 0, 3, &object.Ka[0]);
		env->SetObjectField(_o_instance, _f_Ka, _Ka);
		//	env->ReleaseFloatArrayElements(_Ka, &material.Ka[0], 0);
		env->DeleteLocalRef(_Ka);
		jfloatArray _Kd = env->NewFloatArray(3);
		env->SetFloatArrayRegion(_Kd, 0, 3, &object.Kd[0]);
		env->SetObjectField(_o_instance, _f_Kd, _Kd);
		//	env->ReleaseFloatArrayElements(_Kd, &material.Kd[0], 0);
		env->DeleteLocalRef(_Kd);
		jfloatArray _Ks = env->NewFloatArray(3);
		env->SetFloatArrayRegion(_Ks, 0, 3, &object.Ks[0]);
		env->SetObjectField(_o_instance, _f_Ks, _Ks);
		//	env->ReleaseFloatArrayElements(_Ks, &material.Ks[0], 0);
		env->DeleteLocalRef(_Ks);
		env->SetObjectArrayElement(_o_mtls, i, _o_instance);
		env->DeleteLocalRef(_o_instance);
	}
	return _o_mtls;
}
