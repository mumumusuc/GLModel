#include "GLRender.h"

using namespace vmath;

JNIEXPORT int JNICALL
Java_com_mumu_glmodel_GLRender_createProgram(JNIEnv* env, jobject obj,
		jstring vert, jstring frag) {
	GLint progHandler = ERROR;
	GLint linked;
	progHandler = glCreateProgram();
	if (progHandler == 0)
		return -1;

	Java_com_mumu_glmodel_GLRender_initShader(env, obj, progHandler, vert,
			frag);

	glLinkProgram(progHandler);
	glGetProgramiv(progHandler, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(progHandler, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char *infoLog = static_cast<char*>(malloc(sizeof(char) * infoLen));
			glGetProgramInfoLog(progHandler, infoLen, NULL, infoLog);
			LOGE("Error linking program:[%s]", infoLog);
			free(infoLog);
		}
		glDeleteProgram(progHandler);
		return -2;
	}
	LOGI("create glprog = %d", progHandler);
	return progHandler;
}

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_resizeWindow(JNIEnv* env, jobject obj,
		jint width, jint height) {
	g_width = width;
	g_height = height;
	glViewport(0, 0, g_width, g_height);
}

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_initShader(JNIEnv *env, jobject self, jint prog,
		jstring vertex, jstring fragment) {
	if (!prog) {
		LOGE("initShader -> bad program argument");
		return;
	}
	const char* _vertexShader = env->GetStringUTFChars(vertex, 0);
	const char* _fragmentShader = env->GetStringUTFChars(fragment, 0);
	GLuint vertexShader;
	GLuint fragmentShader;
	vertexShader = loadShader(GL_VERTEX_SHADER, _vertexShader);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, _fragmentShader);
	glAttachShader(prog, vertexShader);
	glAttachShader(prog, fragmentShader);
	env->ReleaseStringUTFChars(vertex, _vertexShader);
	env->ReleaseStringUTFChars(fragment, _fragmentShader);
}

unsigned int v_size, f_size, n_size;

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_loadModel(JNIEnv* env, jobject obj,
		jstring model, jstring mtl) {
	if (!model) {
		LOGE("loadModel -> bad model argment");
		return;
	}
	const char* _model = env->GetStringUTFChars(model, 0);
	ModelLoader mLoader;
	ModelObject object = mLoader.loadObject(_model);

	v_size = object.v.size();
	n_size = object.vn.size();
	f_size = object.fv.size();
	LOGI("vertex_size = %d ,n_size = %d, face_size = %d\n", v_size, n_size,
			f_size);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(
			GL_ARRAY_BUFFER,
			v_size * 3 * sizeof(GLfloat),
			&object.v[0],
			GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(
			GL_ARRAY_BUFFER,
			n_size * 3 * sizeof(GLfloat),
			&object.vn[0],
			GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) NULL);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			f_size * 3 * sizeof(GLuint),
			&object.fv[0],
			GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
	/*	if (out_normals.size()) {
	 glGenBuffers(1, &nbo);
	 glBindBuffer(GL_ARRAY_BUFFER, nbo);
	 glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(vec3),
	 &out_normals[0], GL_STATIC_DRAW);
	 glEnableVertexAttribArray(1);
	 glBindBuffer(GL_ARRAY_BUFFER, nbo);
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	 }
	 if (out_uvs.size()) {
	 glGenBuffers(1, &ubo);
	 glBindBuffer(GL_ARRAY_BUFFER, ubo);
	 glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(vec2),
	 &out_uvs[0], GL_STATIC_DRAW);
	 glEnableVertexAttribArray(2);
	 glBindBuffer(GL_ARRAY_BUFFER, ubo);
	 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	 }*/
	env->ReleaseStringUTFChars(model, _model);
	mLoader.destroyObject(object);
}

int loadTexture(BmpTexture texture) {
	if (texture.img_pixels == NULL) {
		LOGE("load texture failed! invalid image");
		return ERROR;
	}
	LOGI("img_config: data=%s , width=%d , height=%d",
			texture.img_pixels==NULL?"invalid":"valid", texture.img_w,
			texture.img_h);
	glTexImage2D(
	GL_TEXTURE_2D, 0, GL_RGBA, texture.img_w, texture.img_h, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, texture.img_pixels);
	return 1;
}

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_render(JNIEnv* env, jobject obj, jint prog) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(prog);

	float aspect = (float) g_height / g_width;
	mat4 m_proj(
			frustum(-2.0f, 2.0f, -2.0f * aspect, 2.0f * aspect, 10.0f, 800.0f));
	glUniformMatrix4fv(glGetUniformLocation(prog, "m_proj"), 1, GL_FALSE,
			m_proj);

	mat4 m_model = translate(0.0f, 0.0f, -400.0f);
	m_model *= rotate(g_deg, 1.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(glGetUniformLocation(prog, "m_model"), 1, GL_FALSE,
			m_model);

	glUniform1i(glGetUniformLocation(prog, "u_use_light"), GL_TRUE);

	glUniform3f(glGetUniformLocation(prog, "u_light_position"), 0.0f, 0.0f,
			10.0f);
	glUniform3f(glGetUniformLocation(prog, "u_eye_position"), eye_position[0],
			eye_position[1], eye_position[2]);
	glUniform4f(glGetUniformLocation(prog, "u_ambient"), ambient[0], ambient[1],
			ambient[2], ambient[3]);
	glUniform3f(glGetUniformLocation(prog, "u_light_color"), light_color[0],
			light_color[1], light_color[2]);
	glUniform1f(glGetUniformLocation(prog, "u_Ns"), Ns);
	glUniform1f(glGetUniformLocation(prog, "u_attenuation"), attenuation);

	glBindVertexArray(vao);
//	glDrawArrays(GL_TRIANGLES, 0, v_size);
	glDrawElements(GL_TRIANGLES, 3 * f_size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

JNIEXPORT int Java_com_mumu_glmodel_GLRender_loadBitmapTextrue(JNIEnv* env,
		jobject thiz, jobjectArray jbitmaps) {
	jsize size = env->GetArrayLength(jbitmaps);
	if (size == 0)
		return -1;

	AndroidBitmapInfo bitmapInfo;
	BmpTexture texture;

	for (int i = 0; i < size; i++) {
		jobject bmp = env->GetObjectArrayElement(jbitmaps, i);

		if (AndroidBitmap_getInfo(env, bmp, &bitmapInfo) < 0) {
			LOGE("AndroidBitmap_getInfo() failed !");
			return -1;
		}
		texture.img_w = bitmapInfo.width;
		texture.img_h = bitmapInfo.height;

		if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
			LOGE(
					"Java_com_example_hellojni_HelloJni_showBitmap invalid rgb format");
			return -1;
		}

		if (AndroidBitmap_lockPixels(env, bmp, &texture.img_pixels) < 0) {
			LOGE("AndroidBitmap_lockPixels() failed !");
		}

		textures.push_back(texture);
		AndroidBitmap_unlockPixels(env, bmp);
	}
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return 0;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_clean(JNIEnv *pEnv,
		jobject obj) {
	glDeleteBuffers(2, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_rotateModel(JNIEnv *pEnv,
		jobject obj, jfloat deg, jfloat x, jfloat y, jfloat z) {
	g_deg = deg;
	g_x = x;
	g_y = y;
	g_z = z;
}

