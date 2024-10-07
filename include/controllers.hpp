#pragma once

#include "cmd_buff.hpp"
#include <SDL2/SDL_events.h>

namespace cntrl
{
	bool mv_cmds(const Uint8 *key_states, cmd_buff& buff);
}
