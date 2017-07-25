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
	//解析文本
	const char* _model = env->GetStringUTFChars(model, 0);
	ModelObject object = load_coordinates(_model);
	env->ReleaseStringUTFChars(model, _model);
	//重建索引
	const uint v_size = object.fv.size() * 3 * 3;
	const uint t_size = object.ft.size() * 3 * 2;
	const uint n_size = object.fn.size() * 3 * 3;
	LOGI("size = [%d,%d,%d]", v_size, t_size, n_size);
	float v[v_size], t[t_size], n[n_size];
	build_model(object, v, t, n, true);
	//绑定到GPU
	GLuint _handle[4];
	bindBuffers(v, v_size, t, t_size, n, n_size, _handle);
	LOGI("_handle = [%d,%d,%d,%d]", _handle[0], _handle[1], _handle[2],
			_handle[3]);
	//返回绑定结果
	jclass _c_ModelStruct = env->FindClass("com/mumu/glmodel/ModelStruct");
	if (_c_ModelStruct == 0)
		LOGE("error on find class");
	jmethodID _m_init = env->GetMethodID(_c_ModelStruct, "<init>", "()V");
	if (_m_init == 0)
		LOGE("error on get constructor");
	jobject _o_instance = env->NewObject(_c_ModelStruct, _m_init);
	if (_o_instance == 0)
		LOGE("error on new instance");

	jfieldID _f_H_VAO = env->GetFieldID(_c_ModelStruct, "_H_VAO", "I");
	if (_f_H_VAO == 0)
		LOGE("error on get _H_VAO");
	env->SetIntField(_o_instance, _f_H_VAO, _handle[0]);

	jfieldID _f_H_vertex = env->GetFieldID(_c_ModelStruct, "_H_vertex", "I");
	if (_f_H_vertex == 0)
		LOGE("error on get _H_vertex");
	env->SetIntField(_o_instance, _f_H_vertex, _handle[1]);

	jfieldID _f_mVertexSize = env->GetFieldID(_c_ModelStruct, "mVertexSize",
			"I");
	if (_f_mVertexSize == 0)
		LOGE("error on get mVertexSize");
	env->SetIntField(_o_instance, _f_mVertexSize, v_size/3 );

	jfieldID _f_H_texture = env->GetFieldID(_c_ModelStruct, "_H_texture", "I");
	if (_f_H_texture == 0)
		LOGE("error on get _H_texture");
	env->SetIntField(_o_instance, _f_H_texture, _handle[2]);

	jfieldID _f_H_normal = env->GetFieldID(_c_ModelStruct, "_H_normal", "I");
	if (_f_H_normal == 0)
		LOGE("error on get _H_normal");
	env->SetIntField(_o_instance, _f_H_normal, _handle[3]);

	jobjectArray _result = env->NewObjectArray(1, _c_ModelStruct, _o_instance);
	env->DeleteLocalRef(_o_instance);
	return _result;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_render(JNIEnv *env,
		jobject obj, jint prog, jint _vao, jint _size, jint _texture,
		jint _texture_unit) {
	render(prog, _vao, _size, _texture, _texture_unit);
}

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_GLRender_loadBitmapTextrue(
		JNIEnv *env, jobject obj, jobjectArray bmps) {
	jsize size = env->GetArrayLength(bmps);
	if (size == 0)
		return -1;
	AndroidBitmapInfo bitmapInfo;
	BmpTexture texture;
	for (int i = 0; i < size; i++) {
		jobject bmp = env->GetObjectArrayElement(bmps, i);
		if (AndroidBitmap_getInfo(env, bmp, &bitmapInfo) < 0) {
			LOGE("AndroidBitmap_getInfo() failed !");
			return -1;
		}
		texture.img_w = bitmapInfo.width;
		texture.img_h = bitmapInfo.height;
		if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
			LOGE("invalid rgb format");
			return -1;
		}
		if (AndroidBitmap_lockPixels(env, bmp, &texture.img_pixels) < 0) {
			LOGE("AndroidBitmap_lockPixels failed !");
		}
		//	useTexture(texture);
		AndroidBitmap_unlockPixels(env, bmp);
	}
	return 0;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_rotateModel(JNIEnv *env,
		jobject obj, jfloat dx, jfloat dy, jfloat dz, jfloat x, jfloat y,
		jfloat z) {
	rotateModel(dx, dy, dz, x, y, z);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_clean(JNIEnv *, jobject) {
	clean();
}

