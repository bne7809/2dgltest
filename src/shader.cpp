#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.hpp"

shader::shader(const char *vs_path, const char *fs_path)
{
	std::ifstream vs_file;
	std::ifstream fs_file;
	
	vs_file.open(vs_path);
	fs_file.open(fs_path);

	std::stringstream vs_stream;
	std::stringstream fs_stream;

	vs_stream << vs_file.rdbuf();
	fs_stream << fs_file.rdbuf();

	vs_file.close();
	fs_file.close();

	std::string vs_code = vs_stream.str();
	std::string fs_code = fs_stream.str();

	const char *vs_code_cstr = vs_code.c_str();
	const char *fs_code_cstr = fs_code.c_str();

	char info_log[512];
	int success;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vs, 1, &vs_code_cstr, nullptr);
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(vs, 512, nullptr, info_log);
		std::cout << "VERTEX SHADER FAILED\n" << info_log << "\n";
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fs, 1, &fs_code_cstr, nullptr);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(fs, 512, nullptr, info_log);
		std::cout << "FRAGMENT SHADER FAILED\n" << info_log << "\n";
	}

	m_prog = glCreateProgram();

	glAttachShader(m_prog, vs);
	glAttachShader(m_prog, fs);

	glLinkProgram(m_prog);

	glGetProgramiv(m_prog, GL_LINK_STATUS, &success);

	if(!success)
	{
		glGetProgramInfoLog(m_prog, 512, nullptr, info_log);
		std::cout << "PROGRAM LINKING FAILED\n" << info_log << "\n";
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void shader::bind() const
{
	glUseProgram(m_prog);
}

void shader::set_mat4(const char* name, const float* data) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_prog, name), 1, GL_FALSE, data);
}

shader::~shader()
{
	glDeleteProgram(m_prog);
}
