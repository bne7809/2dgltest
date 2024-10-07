#include "transformers.hpp"

namespace trans
{
	bool archer(entity_info &out, entity_cmd curr_cmd)
	{
		const unsigned int is_grounded = (out.state & entity_state::GROUNDED);

		if(curr_cmd == entity_cmd::MV_RIGHT)
			out.state |= entity_state::MV_RIGHT;
		
		if(curr_cmd == entity_cmd::MV_LEFT)
			out.state |= entity_state::MV_LEFT;

		if(curr_cmd == entity_cmd::ST_RIGHT)
			out.state &= (~entity_state::MV_RIGHT);
		
		if(curr_cmd == entity_cmd::ST_LEFT)
			out.state &= (~entity_state::MV_LEFT);

		if(curr_cmd == entity_cmd::ST_JUMP)
			out.state |= entity_state::JMP;

		if(curr_cmd == entity_cmd::MV_JUMP && (out.state & entity_state::JMP))
		{
			out.state &= (~entity_state::JMP);
			if(is_grounded)
			{
				out.vel_y = -out.max_y;
			} else if((out.state & entity_state::COLLX) && (out.vel_y >= 0.0f)) {
				out.state |= entity_state::WLJMP;
			} else if(out.state & entity_state::DBJUMP) {
				out.vel_y = -out.max_y;
				out.state &= (~entity_state::DBJUMP);
			}
		}

		if(curr_cmd == entity_cmd::TR_FIRE && is_grounded)
		{
			out.state = entity_state::ILOCK;
			out.anim = anim_type::FIRE;

			return true;
		}

		if(curr_cmd == entity_cmd::MV_ROLL && is_grounded)
		{
			out.state = entity_state::ILOCK;

			if(out.dir == 1) out.state |= entity_state::MV_RIGHT;
			else out.state |= entity_state::MV_LEFT;

			out.anim = anim_type::ROLL;

			return true;
		}

		return false;
	}
}
