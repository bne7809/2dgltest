#include "sprite_sheet.hpp"
#include "utils.hpp"

sprite_sheet::sprite_sheet(const char *file, int num_x, int num_y, int rwidth, int rheight) 
	: m_fracw(1.0f/num_x), m_frach(1.0f/num_y)
{
	util::tex_create(file, &m_tex);
	
	const float width = static_cast<float>(rwidth);
	const float height = static_cast<float>(rheight);

	float vertices[] = {
		0.0f, 0.0f, -1.0f, 0.0f,
		width, 0.0f, 1.0f, 0.0f,
		0.0f, height, -1.0f, m_frach,

		width, 0.0f, 1.0f, 0.0f,
		width, height, 1.0f, m_frach,
		0.0f, height, -1.0f, m_frach
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void sprite_sheet::render(const shader& s, int row, int col, float x, float y, int dir)
{
	glm::mat4 tex_mat = glm::mat4(static_cast<float>(dir) * m_fracw * 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, (0.5 + static_cast<float>(col)) * m_fracw, static_cast<float>(row) * m_frach, 0.0f, 1.0f);

	glm::mat4 mod_mat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
	mod_mat = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f) * mod_mat;
	
	s.set_mat4("tex_mat", glm::value_ptr(tex_mat));
	s.set_mat4("mod_mat", glm::value_ptr(mod_mat));

	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

sprite_sheet::~sprite_sheet()
{
	glDeleteTextures(1, &m_tex);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}
