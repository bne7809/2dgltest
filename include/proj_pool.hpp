#pragma once

#include "BlockAllocator.hpp"

struct proj_data
{
	float x;
	float y;
	float vx;
	float vy;
	int time;
	int row;
	int frame;
	int start;
	int end;
	int delay;
};

class proj_pool : public BlockAllocator<proj_data, 64, 8>
{
public:
	void create_proj(float x, float y, float vx, float vy, int row, int start, int end, int delay);
	void update(int dt, float df);
};
