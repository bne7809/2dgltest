#pragma once

#include "anim_entity.hpp"
#include "triggers.hpp"
#include "controllers.hpp"
#include "transformers.hpp"
#include "collisions.hpp"
#include "forces.hpp"
#include "state_anim_conv.hpp"
#include "clock.hpp"

class game
{
public:
	game();
	void start();
	~game();
private:
	static const anim_data s_archer_data[9]; 
	static const anim_data s_knight_data[14];

	SDL_Window *m_window;
	SDL_GLContext m_context;
};
