#include "controllers.hpp"

namespace cntrl
{
	bool mv_cmds(const Uint8 *key_states, cmd_buff& buff)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT) {
				return false;
			} else if(e.type == SDL_KEYUP) {
				switch(e.key.keysym.sym)
				{
				case SDLK_a:
					buff.push(entity_cmd::ST_LEFT);
					break;
				case SDLK_d:
					buff.push(entity_cmd::ST_RIGHT);
					break;
				case SDLK_k:
					buff.push(entity_cmd::ST_JUMP);
					break;
				}
			}
		}

		if(key_states[SDL_SCANCODE_A]) buff.push(entity_cmd::MV_LEFT);
		if(key_states[SDL_SCANCODE_D]) buff.push(entity_cmd::MV_RIGHT);
		if(key_states[SDL_SCANCODE_K]) buff.push(entity_cmd::MV_JUMP);
		if(key_states[SDL_SCANCODE_J]) buff.push(entity_cmd::TR_FIRE);
		if(key_states[SDL_SCANCODE_L]) buff.push(entity_cmd::MV_ROLL);

		return true;
	}
}
