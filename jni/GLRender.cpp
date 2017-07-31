#include "include/GLRender.h"

using namespace vmath;

jint g_width;
jint g_height;
mat4 g_model_ges = rotate(0.0f, 1.0f, 0.0f, 0.0f);
vec3 v_model_loc(0.0f, 0.0f, 0.0f);
mat4 g_model_loc = translate(v_model_loc);
mat4 g_proj;
vec3 v_camera_loc(0.0f, 0.0f, 50.0f);
mat4 g_camera = lookat(v_camera_loc, v_model_loc, vec3(0.0f, 1.0f, 0.0f));

static GLfloat light_position[3] = { 0.0f, 0.0f, 4000.0f };
static GLfloat eye_position[3] = { 0.0f, 10.0f, 30.0f };
static GLfloat light_color[3] = { 1.0f, 1.0f, 1.0f };
static GLfloat Ns = 96.0f;
static GLfloat Ka[3] = { 1.f, 1.f, 1.f };
static GLfloat Kd[3] = { 0.8f, 0.8f, 0.8f };
static GLfloat Ks[3] = { 0.05f, 0.05f, 0.05f };
static GLfloat Ni = 1.f;

GLuint createProgram(const char* vert, const char* frag) {
	GLuint progHandler = ERROR;
	GLint linked;
	progHandler = glCreateProgram();
	if (progHandler == 0)
		return ERROR;
	GLuint h_vertexShader = loadShader(GL_VERTEX_SHADER, vert);
	GLuint h_fragmentShader = loadShader(GL_FRAGMENT_SHADER, frag);
	glAttachShader(progHandler, h_vertexShader);
	glAttachShader(progHandler, h_fragmentShader);
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
		return ERROR;
	}
	LOGI("create gl_program = %d", progHandler);
	return progHandler;
}

void resizeWindow(uint width, uint height) {
	g_width = width;
	g_height = height;
	float aspect = (float) g_height / g_width;
	g_proj = frustum(-2.0f, 2.0f, -2.0f * aspect, 2.0f * aspect, 10.f, 8000.0f);
	glViewport(0, 0, g_width, g_height);
}

GLuint loadShader(GLenum type, const char *shaderSrc) {
	GLuint shader;
	GLint compiled;
	shader = glCreateShader(type);
	if (shader == 0) {
		return 0;
	}
	glShaderSource(shader, 1, &shaderSrc, 0);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1) {
			char *infoLog = static_cast<char*>(malloc(sizeof(char) * infoLen));
			glGetShaderInfoLog(shader, infoLen, 0, infoLog);
			LOGE("Error compiling shader:[%s]", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void bindBuffers(GLfloat* vertex, uint v_size, GLfloat* texture, uint t_size,
		GLfloat* normals, uint n_size, GLuint* result) {
	glGenVertexArrays(1, result);
	glBindVertexArray(*result);
	if (v_size) {
		glGenBuffers(1, result + 1);
		glBindBuffer(GL_ARRAY_BUFFER, *(result + 1));
		glBufferData(
		GL_ARRAY_BUFFER, v_size * sizeof(GLfloat), vertex,
		GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(0);
	}
	if (t_size) {
		glGenBuffers(1, result + 2);
		glBindBuffer(GL_ARRAY_BUFFER, *(result + 2));
		glBufferData(
		GL_ARRAY_BUFFER, t_size * sizeof(GLfloat), texture,
		GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(1);
	}
	if (n_size) {
		glGenBuffers(1, result + 3);
		glBindBuffer(GL_ARRAY_BUFFER, *(result + 3));
		glBufferData(
		GL_ARRAY_BUFFER, n_size * sizeof(GLfloat), normals,
		GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0);
}

void render(GLuint prog, bool _clear, GLuint _vao, uint _size, GLuint _texture,
		GLuint _texture_unit, GLfloat _Ns, GLfloat _Ni, GLfloat _Ka0,
		GLfloat _Ka1, GLfloat _Ka2, GLfloat _Kd0, GLfloat _Kd1, GLfloat _Kd2,
		GLfloat _Ks0, GLfloat _Ks1, GLfloat _Ks2) {
	if (_clear) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 源RGB*源A + 背景RGB*1
	glEnable(GL_BLEND); // 设置启用

	glUseProgram(prog);
	glUniformMatrix4fv(glGetUniformLocation(prog, "m_proj"), 1, GL_FALSE,
			g_proj * g_camera);
	glUniformMatrix4fv(glGetUniformLocation(prog, "m_model"), 1, GL_FALSE,
			g_model_loc * g_model_ges);
	glUniform1i(glGetUniformLocation(prog, "u_use_light"), GL_TRUE);
	glUniform3f(glGetUniformLocation(prog, "u_light_position"),
			light_position[0], light_position[1], light_position[2]);
	glUniform3f(glGetUniformLocation(prog, "u_eye_position"), eye_position[0],
			eye_position[1], eye_position[2]);
	//glUniform4f(glGetUniformLocation(prog, "u_ambient"), ambient[0], ambient[1],ambient[2], ambient[3]);
	glUniform3f(glGetUniformLocation(prog, "u_light_color"), light_color[0],
			light_color[1], light_color[2]);
	//input
	if (_Ns > 0) {
		glUniform1f(glGetUniformLocation(prog, "u_Ns"), _Ns);
		glUniform1f(glGetUniformLocation(prog, "u_Ni"), _Ni);
		glUniform3f(glGetUniformLocation(prog, "u_Ka"), _Ka0, _Ka1, _Ka2);
		glUniform3f(glGetUniformLocation(prog, "u_Kd"), _Kd0, _Kd1, _Kd2);
		glUniform3f(glGetUniformLocation(prog, "u_Ks"), _Ks0, _Ks1, _Ks2);
	} else {
		glUniform1f(glGetUniformLocation(prog, "u_Ns"), Ns);
		glUniform1f(glGetUniformLocation(prog, "u_Ni"), Ni);
		glUniform3f(glGetUniformLocation(prog, "u_Ka"), Ka[0], Ka[1], Ka[2]);
		glUniform3f(glGetUniformLocation(prog, "u_Kd"), Kd[0], Kd[1], Kd[2]);
		glUniform3f(glGetUniformLocation(prog, "u_Ks"), Ks[0], Ks[1], Ks[2]);
	}

	glBindVertexArray(_vao);
	if (_texture) {
		glUniform1i(glGetUniformLocation(prog, "u_use_texture"), GL_TRUE);
		glActiveTexture(GL_TEXTURE0 + _texture_unit);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glUniform1i(glGetUniformLocation(prog, "u_sampler"), _texture_unit);
	}
	//	glDrawElements(GL_TRIANGLES, 3 * f_size, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, _size);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void bindTexture(void* pixels, uint w, uint h, uint _unit, GLuint* _texture) {
	if (pixels == 0) {
		LOGE("load texture failed! invalid image");
		return;
	}
	glActiveTexture(GL_TEXTURE0 + _unit);
	glGenTextures(1, _texture);
	glBindTexture(GL_TEXTURE_2D, *_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
	GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void move(int type, float x, float y, float z, float dx, float dy, float dz) {
	if (type == TYPE_MODEL) {
		v_model_loc = vec3(x, y, z);
		g_model_loc = translate(v_model_loc);
		g_model_ges = rotate(dx, 1.f, 0.f, 0.f) * rotate(dy, 0.f, 1.f, 0.f)
				* rotate(dz, 0.f, 0.f, 1.f);
	}
	if (type == TYPE_CAMERA) {
		v_camera_loc = vec3(x, y, z);
		g_camera = lookat(v_camera_loc, v_model_loc, vec3(0.0f, 1.0f, 0.0f));
	}
}

void deleteBuffers(GLuint* _handle) {
	LOGI("deleteBuffers = %d + [%d,%d,%d,%d]", _handle, *(_handle),
			*(_handle + 1), *(_handle + 2), *(_handle + 3));
	glDeleteVertexArrays(1, _handle);
	glBindBuffer(GL_ARRAY_BUFFER, *(_handle + 1));
	glBufferData(GL_ARRAY_BUFFER, 0, (const void*) NULL, GL_STATIC_DRAW);
	glDeleteBuffers(1, _handle + 1);
	glBindBuffer(GL_ARRAY_BUFFER, *(_handle + 2));
	glBufferData(GL_ARRAY_BUFFER, 0, (const void*) NULL, GL_STATIC_DRAW);
	glDeleteBuffers(1, _handle + 2);
	glBindBuffer(GL_ARRAY_BUFFER, *(_handle + 3));
	glBufferData(GL_ARRAY_BUFFER, 0, (const void*) NULL, GL_STATIC_DRAW);
	glDeleteBuffers(1, _handle + 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	free(_handle);
}

void deleteTexture(GLuint* _texture) {
	if (_texture == NULL)
		return;
	glBindTexture(GL_TEXTURE_2D, *_texture);
	glTexImage2D(
	GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, (const void*) NULL);
	glDeleteTextures(1, _texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	free(_texture);
}
