#include "anim_conv.hpp"

namespace anim_conv
{
	int archer(anim_type type)
	{
		switch(type)
		{
		case anim_type::RUNNING:
			return 0;
		case anim_type::IDLE:
			return 5;
		case anim_type::RISING:
			return 7;
		case anim_type::FALLING:
			return 8;
		case anim_type::FIRE:
			return 3;
		case anim_type::DEATH:
			return 1;
		case anim_type::ROLL:
			return 2;
		case anim_type::WALL_SLIDE:
			return 4;
		case anim_type::DOUBLE_JUMP:
			return 6;
		}
	}

	int knight(anim_type type)
	{
		switch(type)
		{
		case anim_type::RUNNING:
			return 1;
		case anim_type::IDLE:
			return 0;
		case anim_type::RISING:
			return 5;
		case anim_type::FALLING:
			return 6;
		case anim_type::FIRE:
			return 2;
		case anim_type::DEATH:
			return 8;
		case anim_type::ROLL:
			return 11;
		case anim_type::WALL_SLIDE:
			return 13;
		case anim_type::DOUBLE_JUMP:
			return 5;
		}
	}
}
