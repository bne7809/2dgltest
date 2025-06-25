#include "transformers.hpp"

namespace trans
{
	bool archer(dyn_entity &out, entity_cmd curr_cmd)
	{
		if(curr_cmd == entity_cmd::MV_RIGHT)
			BIT_ON(out.state, entity_state::MV_RIGHT);
		
		if(curr_cmd == entity_cmd::MV_LEFT)
			BIT_ON(out.state, entity_state::MV_LEFT);

		if(curr_cmd == entity_cmd::ST_RIGHT)
			BIT_OFF(out.state, entity_state::MV_RIGHT);
		
		if(curr_cmd == entity_cmd::ST_LEFT)
			BIT_OFF(out.state, entity_state::MV_LEFT);

		if(curr_cmd == entity_cmd::TR_FIRE)
		{
			BIT_ON(out.state, entity_state::ILOCK);
			BIT_OFF(out.state, entity_state::MV_LEFT);
			BIT_OFF(out.state, entity_state::MV_RIGHT);
			out.anim = anim_type::FIRE;

			return true;
		}

		if(curr_cmd == entity_cmd::MV_ROLL)
		{
			BIT_ON(out.state, entity_state::ILOCK);

			if(!IS_BIT(out.state, entity_state::X_DIR))
				BIT_ON(out.state, entity_state::MV_RIGHT);
			else
				BIT_ON(out.state, entity_state::MV_LEFT);

			out.anim = anim_type::ROLL;

			return true;
		}

		return false;
	}
}
