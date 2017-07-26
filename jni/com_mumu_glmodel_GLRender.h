/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_mumu_glmodel_GLRender */

#ifndef _Included_com_mumu_glmodel_GLRender
#define _Included_com_mumu_glmodel_GLRender
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    createProgram
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_mumu_glmodel_GLRender_createProgram(JNIEnv *,
		jobject, jstring, jstring);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    resizeWindow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_resizeWindow(JNIEnv *,
		jobject, jint, jint);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    loadModel
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT jobjectArray JNICALL Java_com_mumu_glmodel_GLRender_loadModel(
		JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    genRenderParams
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_genRenderParams(JNIEnv *,
		jobject, jobject);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    render
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_render(JNIEnv *, jobject,
		jint, jint, jint, jint, jint);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    loadBitmapTextrue
 * Signature: ([Landroid/graphics/Bitmap;)I
 */
JNIEXPORT jint JNICALL Java_com_mumu_glmodel_GLRender_loadBitmapTextrue(
		JNIEnv *, jobject, jobject, jint);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    rotateModel
 * Signature: (FFFF)V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_rotateModel(JNIEnv *,
		jobject, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat);

/*
 * Class:     com_mumu_glmodel_GLRender
 * Method:    clean
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_clean(JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
