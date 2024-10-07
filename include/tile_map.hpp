#pragma once

#include "sprite_sheet.hpp"

class tile_map
{
public:
	tile_map(const char *img, const char *map, const char *col_map, int map_x, int num_x, int num_y, int rwidth, int rheight, int nbytes);

	void render(const shader& s);

	inline char get_col(float x, float y) const { return m_col_map[(static_cast<int>(x) / m_rw) + (static_cast<int>(y) / m_rh) * m_width]; }
	inline char get_col(int x, int y) const { return m_col_map[x + y * m_width]; }

	inline int get_x(float x) const { return static_cast<int>(x) / m_rw; }
	inline int get_y(float y) const { return static_cast<int>(y) / m_rh; }

	inline float get_fx(int indx) const { return static_cast<float>(indx * m_rw); }
	inline float get_fy(int indy) const { return static_cast<float>(indy * m_rh); }

	~tile_map();
private:
	GLsizei m_tile_count;

	GLuint m_tex;
	GLuint m_vao;
	GLuint m_vbos[2];

	std::string m_col_map;

	const int m_width;
	const int m_rw;
	const int m_rh;
};
