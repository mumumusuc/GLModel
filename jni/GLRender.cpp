#include "GLRender.h"

jint g_width;
jint g_height;
mat4 g_model_ges = rotate(0.0f, 1.0f, 0.0f, 0.0f);
vec3 v_model_loc(0.0f, 0.0f, 0.0f);
mat4 g_model_loc = translate(v_model_loc);
mat4 g_proj;
vec3 v_camera_loc(0.0f, 0.0f, 50.0f);
mat4 g_camera = lookat(v_camera_loc, v_model_loc, vec3(0.0f, 1.0f, 0.0f));

//GLuint vbo[3], vao, h_texture;
//uint v_size;
//std::vector<BmpTexture> g_textures;

GLfloat ambient[4] = { 1.0, 1.0, 1.0, 0.3 };
GLfloat light_color[3] = { 0.8, 0.8, 0.8 };
GLfloat light_position[3] = { 0.0f, 0.0f, 4000.0f };
GLfloat eye_position[3] = { 0.0, 10.0, 30.0 };
GLfloat sky_color[3] = { 0.7, 0.7, 1.0 };
GLfloat ground_color[3] = { 0.2, 0.2, 0.2 };
GLfloat Ns = 8;
GLfloat attenuation = 0.1;

int createProgram(const char* vert, const char* frag) {
	GLint progHandler = ERROR;
	GLint linked;
	progHandler = glCreateProgram();
	if (progHandler == 0)
		return -1;
	initShader(progHandler, vert, frag);

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

void resizeWindow(uint width, uint height) {
	g_width = width;
	g_height = height;
	float aspect = (float) g_height / g_width;
	g_proj = frustum(-2.0f, 2.0f, -2.0f * aspect, 2.0f * aspect, 10.f,
			8000.0f);
	glViewport(0, 0, g_width, g_height);
}

int initShader(GLint prog, const char* vertexShader,
		const char* fragmentShader) {
	GLuint h_vertexShader;
	GLuint h_fragmentShader;
	h_vertexShader = loadShader(GL_VERTEX_SHADER, vertexShader);
	h_fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(prog, h_vertexShader);
	glAttachShader(prog, h_fragmentShader);
	return 0;
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
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo_v;
	if (v_size) {
		glGenBuffers(1, &vbo_v);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_v);
		glBufferData(
		GL_ARRAY_BUFFER, v_size * sizeof(GLfloat), vertex,
		GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(0);
	}
	GLuint vbo_t;
	if (t_size) {
		glGenBuffers(1, &vbo_t);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_t);
		glBufferData(
		GL_ARRAY_BUFFER, t_size * sizeof(GLfloat), texture,
		GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(1);
	}
	GLuint vbo_n;
	if (n_size) {
		glGenBuffers(1, &vbo_n);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
		glBufferData(
		GL_ARRAY_BUFFER, n_size * sizeof(GLfloat), normals,
		GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *) 0);
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0);
	LOGI("bindBuffers -> result = [%d,%d,%d,%d]", vao, vbo_v, vbo_t, vbo_n);
	result[0] = vao;
	result[1] = vbo_v;
	result[2] = vbo_t;
	result[3] = vbo_n;
}

void render(GLuint prog, GLuint _vao, uint _size, GLuint _texture,
		GLuint _texture_unit) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

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
	glUniform4f(glGetUniformLocation(prog, "u_ambient"), ambient[0], ambient[1],
			ambient[2], ambient[3]);
	glUniform3f(glGetUniformLocation(prog, "u_light_color"), light_color[0],
			light_color[1], light_color[2]);
	glUniform1f(glGetUniformLocation(prog, "u_Ns"), Ns);
	glUniform1f(glGetUniformLocation(prog, "u_attenuation"), attenuation);
	glUniform1i(glGetUniformLocation(prog, "u_use_texture"), _texture > 0);

	glBindVertexArray(_vao);
	glActiveTexture(GL_TEXTURE0 + _texture_unit);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(glGetUniformLocation(prog, "u_sampler"), _texture_unit);
	//	glDrawElements(GL_TRIANGLES, 3 * f_size, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, _size);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int loadBitmapTextrue(jobjectArray jbitmaps) {
	return 0;
}

GLuint bindTexture(void* pixels, uint w, uint h, GLuint _unit) {
	if (pixels == 0) {
		LOGE("load texture failed! invalid image");
		return ERROR;
	}
	glActiveTexture(GL_TEXTURE0 + _unit);
	GLuint _texture;
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(
	GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	GL_RGBA,
	GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	return _texture;
}

void rotateModel(float deg_x, float deg_y, float deg_z, float x, float y,
		float z) {
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

void clean() {
	//	glDeleteBuffers(3, vbo);
	//	glDeleteBuffers(1, &ebo);
	//	glDeleteVertexArrays(1, &vao);
	//	g_textures.clear();
	//	vector<BmpTexture>(g_textures).swap(g_textures);
}
