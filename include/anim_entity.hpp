#pragma once

#include "entity_info.hpp"

class anim_entity
{
public:
	anim_entity(const anim_data *data, int num_x);

	void update(int dt);
	void set_anim(int curr);
private:
	anim_data const * const m_data;

	int orig_anim;
	int curr_anim;
	int next_anim;

	const int m_num_x;
	int m_curr_row;
	int m_curr_frame;

	int m_time;
	int m_loop_done;
public:
	inline int loop_done() const { return m_loop_done; }
	inline int get_row() const { return m_curr_row; }
	inline int get_frame() const { return m_curr_frame; }
};
