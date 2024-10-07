#include "proj_pool.hpp"

proj_pool::proj_pool(const char *file, int num_x, int num_y, int rwidth, int rheight, int cap)
	: m_sprites(file, num_x, num_y, rwidth, rheight), m_mem(new proj_data[cap]), m_size(0), m_cap(cap)
{
}

void proj_pool::create_proj(float x, float y, float vel_x, float vel_y, int dir, int row, int start, int end, int delay)
{
	if(m_size < m_cap)
	{
		m_mem[m_size].x = x;
		m_mem[m_size].y = y;
		m_mem[m_size].vel_x = vel_x;
		m_mem[m_size].vel_y = vel_y;
		m_mem[m_size].dir = dir;
		m_mem[m_size].time = 0;
		m_mem[m_size].row = row;
		m_mem[m_size].frame = start;
		m_mem[m_size].start = start;
		m_mem[m_size].end = end;
		m_mem[m_size].delay = delay;

		m_size++;
	}
}

void proj_pool::update(int dt, float df)
{
	for(int i = 0; i < m_size; i++)
	{
		m_mem[i].x += m_mem[i].dir * m_mem[i].vel_x * df;
		m_mem[i].y += m_mem[i].vel_y * df;
	
		if(m_mem[i].time > m_mem[i].delay)
		{
			m_mem[i].frame++;
			if(m_mem[i].frame > m_mem[i].end)
				m_mem[i].frame = m_mem[i].start;
			m_mem[i].time = 0;
		} else {
			m_mem[i].time += dt;
		}
	}
}

void proj_pool::render(const shader& s)
{
	for(int i = 0; i < m_size; i++)
	{
		m_sprites.render(s, m_mem[i].row, m_mem[i].frame, m_mem[i].x, m_mem[i].y, m_mem[i].dir);
	}
}

proj_pool::~proj_pool()
{
	delete[] m_mem;
}
