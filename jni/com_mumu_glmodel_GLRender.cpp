#include "com_mumu_glmodel_GLRender.h"
#include "GLRender.h"
#include "ModelLoader.h"

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_GLRender_createProgram(JNIEnv *env,
		jobject obj, jstring vert, jstring frag) {
	const char* _vertexShader = env->GetStringUTFChars(vert, 0);
	const char* _fragmentShader = env->GetStringUTFChars(frag, 0);
	GLuint prog = createProgram(_vertexShader, _fragmentShader);
	env->ReleaseStringUTFChars(vert, _vertexShader);
	env->ReleaseStringUTFChars(frag, _fragmentShader);
	return prog;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_resizeWindow(JNIEnv *env,
		jobject obj, jint w, jint h) {
	resizeWindow(w, h);
}

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
	jobject _o_instance = env->NewObject(_c_ModelStruct, _m_init);
	if (_o_instance == 0)
		LOGE("error on new instance");
	//解析文本
	const char* _model = env->GetStringUTFChars(model, 0);
	LOGE("loadModel -> parse coordinates now");
	vector<ModelObject>* objects = load_coordinates(_model);
	LOGE("loadModel -> parse coordinates end");
	env->ReleaseStringUTFChars(model, _model);
	vector<Material>* materials = NULL;
	if (mtl) {
		const char* _mtl = env->GetStringUTFChars(mtl, 0);
		LOGE("loadModel -> parse material now");
		materials = load_mtls(_mtl);
		LOGE("loadModel -> parse material end");
		env->ReleaseStringUTFChars(mtl, _mtl);
	}
	const int size = objects->size();
	LOGE("loadModel -> get %d objects", size);
	if (!size) {
		return NULL;
	}
	jobjectArray _result = env->NewObjectArray(size, _c_ModelStruct, NULL);
	for (int i = 0; i < size; i++) {
		ModelObject object = (*objects)[i];
		//重建索引
		const uint v_size = object.v_size;
		const uint t_size = object.t_size;
		const uint n_size = object.n_size;
		LOGI("loadModel -> size = [%d,%d,%d]", v_size, t_size, n_size);
		//返回绑定结果
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
			delete object.v;
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
			delete object.vt;
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
			delete object.vn;
			object.vn = NULL;
			LOGI("loadModel -> for vn freedom !!!");
		}
		env->SetObjectArrayElement(_result, i, _o_instance);
	}
	env->DeleteLocalRef(_o_instance);
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

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_render(JNIEnv *env,
		jobject obj, jint prog, jint _vao, jint _size, jint _texture,
		jint _texture_unit) {
	render(prog, _vao, _size, _texture, _texture_unit);
}

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_GLRender_loadBitmapTextrue(
		JNIEnv *env, jobject obj, jobject bmp, jint unit) {
	if (!bmp) {
		LOGE("loadBitmapTextrue -> bad input args");
		return NULL;
	}
	AndroidBitmapInfo bitmapInfo;
	if (AndroidBitmap_getInfo(env, bmp, &bitmapInfo) < 0) {
		LOGE("AndroidBitmap_getInfo() failed !");
		return NULL;
	}
	uint w = bitmapInfo.width;
	uint h = bitmapInfo.height;
	void* pixels = NULL;
	if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOGE("invalid rgb format");
		return NULL;
	}
	if (AndroidBitmap_lockPixels(env, bmp, &pixels) < 0) {
		LOGE("AndroidBitmap_lockPixels failed !");
	}
	AndroidBitmap_unlockPixels(env, bmp);
	return bindTexture(pixels, w, h, unit > 0 ? unit : 0);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_rotateModel(JNIEnv *env,
		jobject obj, jfloat dx, jfloat dy, jfloat dz, jfloat x, jfloat y,
		jfloat z) {
	rotateModel(dx, dy, dz, x, y, z);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_clean(JNIEnv *, jobject) {
	clean();
}

