#pragma once

#include <glad/glad.h>

class shader
{
public:
	shader(const char *vs_path, const char *fs_path);
	void bind() const;
	void set_mat4(const char* name, const float* data) const;
	~shader();
private:
	GLuint m_prog;
};
