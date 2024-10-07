#pragma once

#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class sprite_sheet
{
public:
	sprite_sheet(const char *file, int num_x, int num_y, int rwidth, int rheight);
	void render(const shader& s, int row, int col, float x, float y, int dir = 1);
	virtual ~sprite_sheet();
private:
	GLuint m_tex;
	GLuint m_vao;
	GLuint m_vbo;

	const float m_fracw;
	const float m_frach;
};
