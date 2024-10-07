#include "tile_map.hpp"

tile_map::tile_map(const char *img, const char *map, const char *col_map, int map_x, int num_x, int num_y, int rwidth, int rheight, int nbytes)
	: m_tile_count(0), m_width(map_x), m_rw(rwidth), m_rh(rheight)
{
	std::string temp;

	std::fstream cmf(col_map);
	while(std::getline(cmf, temp))
	{
		m_col_map += temp;
	}
	cmf.close();

	const float width = static_cast<float>(rwidth);
	const float height = static_cast<float>(rheight);

	const float twidth = 1.0f / num_x;
	const float theight = 1.0f / num_y;

	std::pair<tile_offset*, GLsizei> ret = util::tile_off_pdest_sz(map, width, height, twidth, theight, nbytes);

	tile_offset *off_data = ret.first;
	m_tile_count = ret.second;

	float vertices[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		width, 0.0f, twidth, 0.0f,
		0.0f, height, 0.0f, theight,

		width, 0.0f, twidth, 0.0f,
		width, height, twidth, theight,
		0.0f, height, 0.0f, theight
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, m_vbos);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, m_tile_count * sizeof(tile_offset), off_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glVertexAttribDivisor(1, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	util::tex_create(img, &m_tex);

	delete[] off_data;
}

void tile_map::render(const shader& s)
{
	glm::mat4 mod_mat = glm::ortho(0.0f, 1280.0f, 800.0f, 0.0f, -1.0f, 1.0f);

	s.set_mat4("mod_mat", glm::value_ptr(mod_mat));

	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_tile_count);
}

tile_map::~tile_map()
{
	glDeleteTextures(1, &m_tex);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(2, m_vbos);
}
