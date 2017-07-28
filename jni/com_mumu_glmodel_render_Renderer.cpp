#include "com_mumu_glmodel_render_Renderer.h"
#include "GLRender.h"
#include "ModelLoader.h"

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_render_Renderer_createProgram(
		JNIEnv *env, jobject obj, jstring vert, jstring frag) {
	const char* _vertexShader = env->GetStringUTFChars(vert, 0);
	const char* _fragmentShader = env->GetStringUTFChars(frag, 0);
	GLuint prog = createProgram(_vertexShader, _fragmentShader);
	env->ReleaseStringUTFChars(vert, _vertexShader);
	env->ReleaseStringUTFChars(frag, _fragmentShader);
	return prog;
	return 0;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_Renderer_genRenderParams(
		JNIEnv *, jobject, jobject) {

}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_Renderer_render(JNIEnv *env,
		jobject obj, jint prog, jboolean _clear, jint _vao, jint _size,
		jint _texture, jint _texture_unit, jfloat _Ns, jfloat _Ni, jfloat _Ka0,
		jfloat _Ka1, jfloat _Ka2, jfloat _Kd0, jfloat _Kd1, jfloat _Kd2,
		jfloat _Ks0, jfloat _Ks1, jfloat _Ks2) {
	render(prog, _clear, _vao, _size, _texture, _texture_unit, _Ns, _Ni, _Ka0,
			_Ka1, _Ka2, _Kd0, _Kd1, _Kd2, _Ks0, _Ks1, _Ks2);
}

JNIEXPORT jint JNICALL Java_com_mumu_glmodel_render_Renderer_loadBitmapTextrue(
		JNIEnv *env, jobject obj, jobject bmp, jint unit) {
	if (!bmp || unit < 0) {
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

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_Renderer_resizeWindow(
		JNIEnv *env, jobject obj, jint w, jint h) {
	resizeWindow(w, h);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_Renderer_rotateModel(
		JNIEnv *env, jobject obj, jfloat dx, jfloat dy, jfloat dz, jfloat x,
		jfloat y, jfloat z) {
	rotateModel(dx, dy, dz, x, y, z);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_Renderer_clean(JNIEnv *,
		jobject) {

}

