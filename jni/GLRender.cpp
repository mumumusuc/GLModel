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
			glGetProgramInfoLog(progHandler, infoLen, 0, infoLog);
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
	float aspect = (float) g_height / g_width;
	g_proj = frustum(-2.0f, 2.0f, -2.0f * aspect, 2.0f * aspect, 10.0f,
			8000.0f);
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

unsigned int v_size, f_size, n_size, t_size;

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
	t_size = object.vt.size();
	f_size = object.fv.size();

	LOGI(
			"vertex_size = %d ,normal_size = %d, texture_size = %d, face_size = %d\n",
			v_size, n_size, t_size, f_size);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(3, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(
	GL_ARRAY_BUFFER, v_size * 3 * sizeof(GLfloat), &object.v[0],
	GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(
	GL_ARRAY_BUFFER, n_size * 3 * sizeof(GLfloat), &object.vn[0],
	GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(
	GL_ARRAY_BUFFER, t_size * 2 * sizeof(GLfloat), &object.vt[0],
	GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(
	GL_ELEMENT_ARRAY_BUFFER, f_size * 3 * sizeof(GLuint), &object.fv[0],
	GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
	env->ReleaseStringUTFChars(model, _model);
	mLoader.destroyObject(object);
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

	glUniformMatrix4fv(glGetUniformLocation(prog, "m_proj"), 1, GL_FALSE,
			g_proj * g_camera);
	glUniformMatrix4fv(glGetUniformLocation(prog, "m_model"), 1, GL_FALSE,
			g_model_loc * g_model_ges);

	glUniform1i(glGetUniformLocation(prog, "u_use_light"), GL_FALSE);

	glUniform3f(glGetUniformLocation(prog, "u_light_position"),
			light_position[0], light_position[1], light_position[2]);
	glUniform3f(glGetUniformLocation(prog, "u_eye_position"), eye_position[0],
			eye_position[1], eye_position[2]);
	glUniform4f(glGetUniformLocation(prog, "u_ambient"), ambient[0], ambient[1],
			ambient[2], ambient[3]);
	glUniform3f(glGetUniformLocation(prog, "u_light_color"), light_color[0],
			light_color[1], light_color[2]);
	glUniform1f(glGetUniformLocation(prog, "u_Ns"), Ns);
	glUniform1f(glGetUniformLocation(prog, "u_attenuation"), attenuation);
	glUniform1i(glGetUniformLocation(prog, "u_use_texture"), GL_TRUE);

	glBindVertexArray(vao);
//	glDrawArrays(GL_TRIANGLES, 0, v_size);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, h_texture);
	glUniform1i(glGetUniformLocation(prog, "u_sampler"), 0);
	glDrawElements(GL_TRIANGLES, 3 * f_size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
			LOGE("invalid rgb format");
			return -1;
		}
		if (AndroidBitmap_lockPixels(env, bmp, &texture.img_pixels) < 0) {
			LOGE("AndroidBitmap_lockPixels failed !");
		}
		g_textures.push_back(texture);
		useTexture(texture);
		AndroidBitmap_unlockPixels(env, bmp);
	}
	return 0;
}

int useTexture(BmpTexture texture) {
	if (texture.img_pixels == 0) {
		LOGE("load texture failed! invalid image");
		return ERROR;
	}
	LOGI("img_config: data=%s , width=%d , height=%d",
			texture.img_pixels == 0 ? "invalid" : "valid", texture.img_w,
			texture.img_h);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &h_texture);
	glBindTexture(GL_TEXTURE_2D, h_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
	GL_TEXTURE_2D, 0, GL_RGBA, texture.img_w, texture.img_h, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, texture.img_pixels);
//	free(texture.img_pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	return 0;
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_rotateModel(JNIEnv *pEnv,
		jobject obj, jfloat deg_x, jfloat deg_y, jfloat deg_z, jfloat x,
		jfloat y, jfloat z) {
	float _x = x == 0 ? 0 : x / abs(x);
	float _y = y == 0 ? 0 : y / abs(y);
	float _z = z == 0 ? 0 : z / abs(z);
	g_model_ges = rotate(deg_x, _x, 0.0f, 0.0f) * rotate(deg_y, 0.0f, _y, 0.0f)
			* g_model_ges;
	if (deg_z != 0) {
		mat4 trans = translate(deg_z * normalize(v_model_loc - v_camera_loc));
		g_camera = trans * g_camera;
		v_camera_loc = vec3(g_camera[3][0], g_camera[3][1], g_camera[3][2]);
	}
}

JNIEXPORT void JNICALL Java_com_mumu_glmodel_GLRender_clean(JNIEnv *pEnv,
		jobject obj) {
	glDeleteBuffers(3, vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	for (int i = 0; i < g_textures.size(); i++) {
		free(g_textures[i].img_pixels);
	}
	g_textures.clear();
	vector<BmpTexture>(g_textures).swap(g_textures);
}
