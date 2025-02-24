/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mumu_glmodel_render_NativeRenderer */

#ifndef _Included_com_mumu_glmodel_render_NativeRenderer
#define _Included_com_mumu_glmodel_render_NativeRenderer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    createProgram
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_mumu_glmodel_render_NativeRenderer_createProgram(
		JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    bindBuffers
 * Signature: (Lcom/mumu/glmodel/model/ModelCoordinate;)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_bindBuffers(
		JNIEnv *, jobject, jobject);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    bindTexture
 * Signature: (Lcom/mumu/glmodel/model/ModelMtl;)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_bindTexture(
		JNIEnv *, jobject, jobject);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    render
 * Signature: (IZIIIIFFFFFFFFFFF)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_render(
		JNIEnv *, jobject, jint, jboolean, jint, jint, jint, jint, jfloat,
		jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat,
		jfloat);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    resizeWindow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_resizeWindow(
		JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    moveModel
 * Signature: (FFFFFF)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_moveModel(
		JNIEnv *, jobject, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    moveCamera
 * Signature: (FFFFFF)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_moveCamera(
		JNIEnv *, jobject, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat);

/*
 * Class:     com_mumu_glmodel_render_NativeRenderer
 * Method:    clear
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_render_NativeRenderer_clear(
		JNIEnv *, jobject, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
