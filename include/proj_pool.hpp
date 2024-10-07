#pragma once

#include "sprite_sheet.hpp"

struct proj_data
{
	float x;
	float y;
	float vel_x;
	float vel_y;
	int dir;
	int time;
	int row;
	int frame;
	int start;
	int end;
	int delay;
};

class proj_pool
{
public:
	proj_pool(const char *file, int num_x, int num_y, int rwidth, int rheight, int cap);
	void create_proj(float x, float y, float vel_x, float vel_y, int dir, int row, int start, int end, int delay);
	void update(int dt, float df);
	void render(const shader& s);
	~proj_pool();
private:
	sprite_sheet m_sprites;
	proj_data *m_mem;
	int m_size;
	int m_cap;
};
