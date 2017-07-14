#include "GLRender.h"

using namespace vmath;

JNIEXPORT int JNICALL
Java_com_mumu_glmodel_GLRender_createProgram(JNIEnv* env, jobject obj) {
	GLint progHandler = ERROR;
	GLint linked;
	// Create the program object
	progHandler = glCreateProgram();
	if (progHandler == 0)
		return ERROR;
	// Link the program
	glLinkProgram(progHandler);
	// Check the link status
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
		return ERROR;
	}
	return progHandler;
}

JNIEXPORT void JNICALL
Java_com_mumu_opengldemo_GLRender_resizeWindow(JNIEnv* env, jobject obj,
		jint width, jint height) {
	g_width = width;
	g_height = height;
}

JNIEXPORT void JNICALL
Java_com_mumu_opengldemo_GLRender_initShader(JNIEnv *env, jobject self,
		jint prog, jstring vertex, jstring fragment) {
	if (!prog) {
		LOGE("initShader -> bad program argument");
		return;
	}
	g_progHandler = prog;
	const char* _vertexShader = env->GetStringUTFChars(vertex, 0);
	const char* _fragmentShader = env->GetStringUTFChars(fragment, 0);
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, _vertexShader);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, _fragmentShader);
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	env->ReleaseStringUTFChars(vertex, _vertexShader);
	env->ReleaseStringUTFChars(fragment, _fragmentShader);
}

JNIEXPORT void JNICALL
Java_com_mumu_glmodel_GLRender_loadModel(JNIEnv* env, jobject obj,
		jstring model, jstring mtl) {
	if (!model) {
		LOGE("loadModel -> bad model argment");
		return;
	}
	const char* _model = env->GetStringUTFChars(model, 0);
	loadOBJ(_model, out_vertices, out_uvs, out_normals, out_mtlIndex);
	env->ReleaseStringUTFChars(model, _model);
	if (mtl) {
		const char* _mtl = env->GetStringUTFChars(mtl, 0);
		loadMtl(_mtl, out_mtls);
		env->ReleaseStringUTFChars(mtl, _mtl);
	}
	// LOGI("vertex size = %d \n", out_vertices.size());
	// LOGI("uv size = %d \n", out_uvs.size());
	// LOGI("normal size = %d \n", out_normals.size());
	// LOGI("mtlIndex size = %d \n",out_mtlIndex.size());
	// LOGI("mtl size = %d \n",out_mtls.size());
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(vec3),
			&out_vertices[0], GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	if (out_normals.size() > 0) {
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(vec3),
				&out_normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	if (out_uvs.size() > 0) {
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_ARRAY_BUFFER, ubo);
		glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(vec2),
				&out_uvs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, ubo);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}
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
Java_com_mumu_opengldemo_GLRender_render(JNIEnv* env, jobject obj) {
	glViewport(0, 0, g_width, g_height);
	glClear( GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(g_progHandler);
	GLint project_loc = glGetUniformLocation(g_progHandler,
			"projection_matrix");
	float aspect = g_height / (float) g_width;
	mat4 projection_matrix(
			frustum(-1.0f, 1.0f, aspect, aspect, 100.0f, 1000.0f));
	//mat4 projection_matrix = vmath::lookat(vec3(0,0,0),vec3(0,0,-100.0f),vec3(0,1.0f,0));
	//projection_matrix *= rotate(g_deg,g_x,g_y,g_z) ;//* projection_matrix;
	glUniformMatrix4fv(project_loc, 1, GL_FALSE, projection_matrix);
	GLint model_loc = glGetUniformLocation(g_progHandler, "model_matrix");
	mat4 mModel = translate(0.0f, -2.0f, -380.0f);
	mModel *= rotate(g_deg, 0.0f, 1.0f, 0.0f);
	//mModel *= rotate(30.0f,1.0f,0.0f,0.0f);
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, mModel);

	GLint _enableLight = glGetUniformLocation(g_progHandler, "enableLight");
	GLint _enableTexture = glGetUniformLocation(g_progHandler, "enableTexture");
	for (int i = 0; i < out_mtlIndex.size(); i++) {
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		if (out_normals.size() > 0) {
			glUniform1i(_enableLight, 1);
			GLint light_pos = glGetUniformLocation(g_progHandler,
					"LightPosition");
			vec3 mLightPos(-0.0f, 0.0f, 0.0f);
			glUniform3fv(light_pos, 1, mLightPos);
			//光源
			GLint Ld_pos = glGetUniformLocation(g_progHandler, "Ld");
			vec3 mLd(1.0f, 1.0f, 1.0f);
			glUniform3fv(Ld_pos, 1, mLd);
			//材质
			int mtlCode = (int) out_mtlIndex[i][0];
			GLint Ns_pos = glGetUniformLocation(g_progHandler, "Ns");
			glUniform1f(Ns_pos, out_mtls[mtlCode].Ns);
			GLint ka_pos = glGetUniformLocation(g_progHandler, "Ka");
			glUniform3fv(ka_pos, 1, out_mtls[mtlCode].Ka);
			GLint kd_pos = glGetUniformLocation(g_progHandler, "Kd");
			glUniform3fv(kd_pos, 1, out_mtls[mtlCode].Kd);
			GLint ks_pos = glGetUniformLocation(g_progHandler, "Ks");
			glUniform3fv(ks_pos, 1, out_mtls[mtlCode].Ks);
		} else
			glUniform1i(_enableLight, 0);
		//textrue
		glUniform1i(_enableTexture, 0);
		if (out_uvs.size() > 0) {
			int mtl_index = out_mtls[(int) out_mtlIndex[i][0]].mtl_index;
			if (mtl_index != -1) {
				glUniform1i(_enableTexture, 1);
				loadTexture(textures[mtl_index]);
				GLint tex = glGetUniformLocation(g_progHandler, "Tex1");
				glUniform1i(tex, 0);
			} else {
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDepthMask(GL_FALSE); // 将深度缓存设为只读
			}
		}
		glDrawArrays(GL_TRIANGLES, 3 * (int) out_mtlIndex[i][1],
				3 * (int) out_mtlIndex[i][2]);
	}
}

JNIEXPORT int Java_com_mumu_opengldemo_GLRender_loadBitmapTextrue(JNIEnv* env,
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

JNIEXPORT void JNICALL Java_com_mumu_opengldemo_GLRender_glesClean(JNIEnv *pEnv,
		jobject obj) {
	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

JNIEXPORT void JNICALL Java_com_mumu_opengldemo_GLRender_glesRotateModel(
		JNIEnv *pEnv, jobject obj, jfloat deg, jfloat x, jfloat y, jfloat z) {
	g_deg = deg;
	g_x = x;
	g_y = y;
	g_z = z;
}

