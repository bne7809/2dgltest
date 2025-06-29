#include "anim_entity.hpp"

anim_entity::anim_entity(const anim_data *data, int num_x)
	: m_data(data), orig_anim(0), curr_anim(0), next_anim(data[0].next),
	m_num_x(num_x), m_curr_row(0), m_curr_frame(0), m_time(0), m_loop_done(0) 
{
}

void anim_entity::update(int dt)
{
	if (m_time > m_data[curr_anim].delay && !m_loop_done)
	{
		const int start_frame = m_data[curr_anim].start;
		const int start_row = m_data[curr_anim].row;
		const int tot_frames = m_data[curr_anim].nframes;

		m_curr_frame++;
		if(m_curr_frame >= m_num_x) {
			m_curr_frame = 0;
			m_curr_row++;
		}

		const int row_diff = m_curr_row - start_row;
		const int frame_diff = m_curr_frame - start_frame;
		const int start_diff = m_num_x - start_frame;

		const int curr_nframes = (start_diff + m_curr_frame) * (row_diff == 1) + frame_diff * (row_diff == 0) + 1;

		if (curr_nframes > tot_frames)
		{
			if(next_anim != -1) {
				m_curr_frame = start_frame;
				m_curr_row = start_row;
			}

			if(m_data[curr_anim].tr != nullptr) m_data[curr_anim].tr(this);

			if(next_anim != -1) curr_anim = next_anim;
			else m_loop_done = 1;
		}
		m_time = 0;
	} else {
		m_time += dt;
	}
}

void anim_entity::set_anim(int curr)
{
	if(orig_anim != curr || (next_anim == -1 && m_loop_done))
	{
		orig_anim = curr;
		curr_anim = curr;
		next_anim = m_data[curr].next;
		m_curr_row = m_data[curr].row;
		m_curr_frame = m_data[curr].start;
		m_time = 0;
		m_loop_done = 0;
	}
}
