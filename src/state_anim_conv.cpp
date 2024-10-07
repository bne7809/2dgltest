#include "state_anim_conv.hpp"

namespace state_anim
{
	void archer(entity_info& out)
	{
		if(out.state & entity_state::GROUNDED)
		{
			if(!(out.state & entity_state::MV_LEFT) != !(out.state & entity_state::MV_RIGHT))
				out.anim = anim_type::RUNNING;
			else
				out.anim = anim_type::IDLE;
		} else {
			if(out.vel_y >= 0.0f)
			{
				if(out.state & entity_state::COLLX)
					out.anim = anim_type::WALL_SLIDE;
				else
					out.anim = anim_type::FALLING;
			} else {
				if(out.state & entity_state::DBJUMP)
					out.anim = anim_type::RISING;
				else
					out.anim = anim_type::DOUBLE_JUMP;
			}
		}
	}
}
