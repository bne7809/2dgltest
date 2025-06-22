#include "state_anim_conv.hpp"

namespace state_anim
{
	void archer(dyn_entity& out)
	{
		if(!IS_BIT(out.state, entity_state::MV_LEFT) != !IS_BIT(out.state, entity_state::MV_RIGHT))
			out.anim = anim_type::RUNNING;
		else
			out.anim = anim_type::IDLE;
	}
}
