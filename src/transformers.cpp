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

		return false;
	}
}
