#include "proj_pool.hpp"

void proj_pool::create_proj(float x, float y, float vx, float vy, int row, int start, int end, int delay)
{
	proj_data *p = alloc();

	p->x = x;
	p->y = y;
	p->vx = vx;
	p->vy = vy;
	p->time = 0;
	p->row = row;
	p->frame = start;
	p->start = start;
	p->end = end;
	p->delay = delay;
}

void proj_pool::update(int dt, float df)
{
	update_del_if([dt, df](proj_data &data)
			{
				data.x += data.vx * df;
				data.y += data.vy * df;

				if(data.time > data.delay)
				{
					data.frame++;
					if(data.frame > data.end)
						data.frame = data.start;
					data.time = 0;
				}
				else
					data.time += dt;
			},
			[](proj_data &data)
			{
				if(data.x < 0.0f || data.x >= 1280.0f)
					return true;
				if(data.y < 0.0f || data.y >= 72.0f)
					return true;
				return false;
			}
			);
}
