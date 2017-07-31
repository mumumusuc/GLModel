#include "include/com_mumu_glmodel_render_Renderer.h"
#include "include/GLRender.h"
#include "include/ModelLoader.h"

#define TYPE_VAO 1000
#define TYPE_TEXTURE 1001

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_render_NativeRenderer_createProgram(
		JNIEnv *env, jobject obj, jstring vert, jstring frag) {
	const char* _vertexShader = env->GetStringUTFChars(vert, 0);
	const char* _fragmentShader = env->GetStringUTFChars(frag, 0);
	GLuint prog = createProgram(_vertexShader, _fragmentShader);
	env->ReleaseStringUTFChars(vert, _vertexShader);
	env->ReleaseStringUTFChars(frag, _fragmentShader);
	return prog;
	return 0;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_bindBuffers(
		JNIEnv *env, jobject obj, jobject c) {
	if (!c) {
		LOGE("bindBuffers -> bad input arg");
		return;
	}
	jclass _c_ModelCo = env->GetObjectClass(c);
	jfieldID _f_mVertexSize = env->GetFieldID(_c_ModelCo, "mVertexSize", "I");
	if (_f_mVertexSize == 0)
		LOGE("bindBuffers -> error on get mVertexSize");
	jint v_size = env->GetIntField(c, _f_mVertexSize);
	jfloatArray v = NULL;
	jfloat* _v = NULL;
	jfieldID _f_mVertexCoBuffer = env->GetFieldID(_c_ModelCo, "mVertexCoBuffer",
			"[F");
	if (_f_mVertexCoBuffer == 0)
		LOGE("bindBuffers -> error on get mVertexCoBuffer");
	if (v_size) {
		v = (jfloatArray) env->GetObjectField(c, _f_mVertexCoBuffer);
		_v = env->GetFloatArrayElements(v, 0);
	}
	jfieldID _f_mTextureSize = env->GetFieldID(_c_ModelCo, "mTextureSize", "I");
	if (_f_mTextureSize == 0)
		LOGE("bindBuffers -> error on get mTextureSize");
	jint t_size = env->GetIntField(c, _f_mTextureSize);
	jfloat* _t = NULL;
	jfloatArray t = NULL;
	jfieldID _f_mTextureCoBuffer = env->GetFieldID(_c_ModelCo,
			"mTextureCoBuffer", "[F");
	if (_f_mTextureCoBuffer == 0)
		LOGE("bindBuffers -> error on get mTextureCoBuffer");
	if (t_size) {
		t = (jfloatArray) env->GetObjectField(c, _f_mTextureCoBuffer);
		_t = env->GetFloatArrayElements(t, 0);
	}
	jfieldID _f_mNormalSize = env->GetFieldID(_c_ModelCo, "mNormalSize", "I");
	if (_f_mNormalSize == 0)
		LOGE("bindBuffers -> error on get mNormalSize");
	jint n_size = env->GetIntField(c, _f_mNormalSize);
	jfloat* _n = NULL;
	jfloatArray n = NULL;
	jfieldID _f_mNormalCoBuffer = _f_mNormalCoBuffer = env->GetFieldID(
			_c_ModelCo, "mNormalCoBuffer", "[F");
	if (_f_mNormalCoBuffer == 0)
		LOGE("error on get mNormalCoBuffer");
	if (n_size) {
		n = (jfloatArray) env->GetObjectField(c, _f_mNormalCoBuffer);
		_n = env->GetFloatArrayElements(n, 0);
	}
	//绑定到GPU
	GLuint *_handle = (GLuint*) malloc(STRIDE * sizeof(GLuint));
	bindBuffers(_v, v_size, _t, t_size, _n, n_size, _handle);
	LOGI("bindBuffers -> _handle = %d + [%d,%d,%d,%d]", _handle, _handle[0],
			_handle[1], _handle[2], _handle[3]);
	jfieldID _f_H_BUFFERS_ADDR = env->GetFieldID(_c_ModelCo, "_H_BUFFERS_ADDR",
			"I");
	if (_f_H_BUFFERS_ADDR == 0)
		LOGE("bindBuffers -> error on get _H_BUFFERS_ADDR");
	env->SetIntField(c, _f_H_BUFFERS_ADDR, (jint) _handle);
	LOGE("bindBuffers -> set _H_BUFFERS_ADDR");
	jfieldID _f_H_VAO = env->GetFieldID(_c_ModelCo, "_H_VAO", "I");
	if (_f_H_VAO == 0)
		LOGE("bindBuffers -> error on get _H_VAO");
	env->SetIntField(c, _f_H_VAO, _handle[0]);
	LOGE("bindBuffers -> set _f_H_VAO");
	jfieldID _f_H_VERTEX_CO = env->GetFieldID(_c_ModelCo, "_H_VERTEX_CO", "I");
	if (_f_H_VERTEX_CO == 0)
		LOGE("bindBuffers -> error on get _H_VERTEX_CO");
	env->SetIntField(c, _f_H_VERTEX_CO, _handle[1]);
	LOGE("bindBuffers -> set _f_H_VERTEX_CO");
	jfieldID _f_H_TEXTURE_CO = env->GetFieldID(_c_ModelCo, "_H_TEXTURE_CO",
			"I");
	if (_f_H_TEXTURE_CO == 0)
		LOGE("bindBuffers -> error on get _H_TEXTURE_CO");
	env->SetIntField(c, _f_H_TEXTURE_CO, _handle[2]);
	LOGE("bindBuffers -> set _f_H_TEXTURE_CO");
	jfieldID _f_H_NORMAL_CO = env->GetFieldID(_c_ModelCo, "_H_NORMAL_CO", "I");
	if (_f_H_NORMAL_CO == 0)
		LOGE("bindBuffers -> error on get _f_H_NORMAL_CO");
	env->SetIntField(c, _f_H_NORMAL_CO, _handle[3]);
	LOGE("bindBuffers -> set _f_H_NORMAL_CO");
	if (v && _v) {
		env->ReleaseFloatArrayElements(v, _v, 0);
		env->DeleteLocalRef(v);
		LOGE("delete v");
	}
	if (t && _t) {
		env->ReleaseFloatArrayElements(t, _t, 0);
		env->DeleteLocalRef(t);
		LOGE("delete t");
	}
	if (n && _n) {
		env->ReleaseFloatArrayElements(n, _n, 0);
		env->DeleteLocalRef(n);
		LOGE("delete n");
	}
	//env->SetObjectField(model, _f_mVertexCoBuffer, NULL);
	//env->SetObjectField(model, _f_mTextureCoBuffer, NULL);
	//env->SetObjectField(model, _f_mNormalCoBuffer, NULL);
}

void loadBitmapTextrue(JNIEnv *env, jobject obj, jobject bmp, uint unit,
		GLuint* texture) {
	if (!bmp || unit < 0) {
		LOGE("loadBitmapTextrue -> bad input args");
		return;
	}
	AndroidBitmapInfo bitmapInfo;
	if (AndroidBitmap_getInfo(env, bmp, &bitmapInfo) < 0) {
		LOGE("AndroidBitmap_getInfo() failed !");
		return;
	}
	uint w = bitmapInfo.width;
	uint h = bitmapInfo.height;
	void* pixels = NULL;
	if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOGE("invalid rgb format");
		return;
	}
	if (AndroidBitmap_lockPixels(env, bmp, &pixels) < 0) {
		LOGE("AndroidBitmap_lockPixels failed !");
	}
	AndroidBitmap_unlockPixels(env, bmp);
	bindTexture(pixels, w, h, unit > 0 ? unit : 0, texture);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_bindTexture(
		JNIEnv *env, jobject obj, jobject m) {
	if (!m) {
		LOGE("bindTexture -> bad input arg");
		return;
	}
	jclass _c_ModelMtl = env->GetObjectClass(m);
	jfieldID _f_mTexture = env->GetFieldID(_c_ModelMtl, "mTexture",
			"Landroid/graphics/Bitmap;");
	jobject bmp = env->GetObjectField(m, _f_mTexture);
	jfieldID _f_H_TEXTURE_UNIT = env->GetFieldID(_c_ModelMtl, "_H_TEXTURE_UNIT",
			"I");
	jint unit = env->GetIntField(m, _f_H_TEXTURE_UNIT);
	GLuint *_texture = (GLuint*) malloc(sizeof(GLuint));
	loadBitmapTextrue(env, obj, bmp, unit, _texture);
	LOGE("bindTexture -> gen texture handle = %d", _texture);
	jfieldID _f_H_TEXTURE = env->GetFieldID(_c_ModelMtl, "_H_TEXTURE", "I");
	env->SetIntField(m, _f_H_TEXTURE, *_texture);
	jfieldID _f_H_TEXTURE_ADDR = env->GetFieldID(_c_ModelMtl, "_H_TEXTURE_ADDR",
			"I");
	env->SetIntField(m, _f_H_TEXTURE_ADDR, (jint) _texture);
	env->DeleteLocalRef(bmp);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_render(
		JNIEnv *env, jobject obj, jint prog, jboolean _clear, jint _vao,
		jint _size, jint _texture, jint _texture_unit, jfloat _Ns, jfloat _Ni,
		jfloat _Ka0, jfloat _Ka1, jfloat _Ka2, jfloat _Kd0, jfloat _Kd1,
		jfloat _Kd2, jfloat _Ks0, jfloat _Ks1, jfloat _Ks2) {
	render(prog, _clear, _vao, _size, _texture, _texture_unit, _Ns, _Ni, _Ka0,
			_Ka1, _Ka2, _Kd0, _Kd1, _Kd2, _Ks0, _Ks1, _Ks2);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_resizeWindow(
		JNIEnv *env, jobject obj, jint w, jint h) {
	resizeWindow(w, h);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_moveModel(
		JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z, jfloat dx,
		jfloat dy, jfloat dz) {
	move(TYPE_MODEL, x, y, z, dx, dy, dz);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_moveCamera(
		JNIEnv *env, jobject obj, jfloat x, jfloat y, jfloat z, jfloat dx,
		jfloat dy, jfloat dz) {
	move(TYPE_CAMERA, x, y, z, dx, dy, dz);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_clear(
		JNIEnv *env, jobject obj, jint type, jint values_addr) {
	LOGI("clear -> type = %d, addr = %d", type, values_addr);
	if (type == TYPE_VAO && values_addr != NULL)
		deleteBuffers((GLuint*) values_addr);
	if (type == TYPE_TEXTURE && values_addr != NULL)
		deleteTexture((GLuint*) values_addr);
}

