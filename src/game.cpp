#include "game.hpp"

const anim_data game::s_archer_data[] = 
{
	{0, 0, 7, 100000, 0, nullptr},
	{1, 0, 7, 200000, 1, nullptr},
	{2, 0, 6, 100000, -1, nullptr},
	{3, 0, 6, 150000, -1, trigger::pink_arrow},
	{4, 0, 5, 100000, 4, nullptr},
	{5, 0, 3, 150000, 5, nullptr},
	{6, 0, 3, 100000, 7, nullptr},
	{7, 0, 0, 0, 7, nullptr},
	{7, 1, 1, 0, 8, nullptr}
};

const anim_data game::s_knight_data[] =
{
	{0, 0, 8, 150000, 0, nullptr},
	{0, 8, 10, 100000, 1, nullptr},
	{1, 8, 6, 100000, -1, nullptr},
	{2, 4, 6, 100000, 3, nullptr},
	{3, 0, 8, 100000, 4, nullptr},
	{3, 8, 3, 200000, -1, nullptr},
	{4, 1, 4, 100000, 6, nullptr},
	{4, 5, 3, 100000, -1, nullptr},
	{4, 8, 10, 100000, -1, nullptr},
	{5, 8, 8, 100000, 9, nullptr},
	{6, 6, 5, 100000, -1, nullptr},
	{7, 1, 9, 100000, -1, nullptr},
	{8, 0, 5, 100000, -1, nullptr},
	{8, 5, 5, 100000, 13, nullptr}
};

game::game()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	IMG_Init(IMG_INIT_PNG);

	m_window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_OPENGL);
	m_context = SDL_GL_CreateContext(m_window);

	glViewport(0, 0, 1280, 800);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void print_cmds(const cmd_buff& c)
{
	if(!c.empty())
	{
		SDL_Log("CBEGIN\n");
		for(int i = 0; i < c.num_cmds(); i++)
		{
			switch(c.get_cmd(i))
			{
			case entity_cmd::MV_JUMP:
				SDL_Log("MV_JUMP\n");
				break;
			case entity_cmd::ST_JUMP:
				SDL_Log("ST_JUMP\n");
				break;
			case entity_cmd::ST_LEFT:
				SDL_Log("ST_LEFT\n");
				break;
			case entity_cmd::ST_RIGHT:
				SDL_Log("ST_RIGHT\n");
				break;
			case entity_cmd::MV_ROLL:
				SDL_Log("MV_ROLL\n");
				break;
			case entity_cmd::TR_FIRE:
				SDL_Log("TR_FIRE\n");
				break;
			case entity_cmd::MV_LEFT:
				SDL_Log("MV_LEFT\n");
				break;
			case entity_cmd::MV_RIGHT:
				SDL_Log("MV_RIGHT\n");
				break;
			}
		}
		SDL_Log("CEND\n");
	}
}

void game::start()
{
	cmd_buff cmds;

	shader anim_prog("shaders/anim_sprite.vs", "shaders/anim_sprite.fs");
	shader tile_prog("shaders/tiles.vs", "shaders/anim_sprite.fs");
	
	//entity_info archer_info = {200.0f, 500.0f, 0.0f, 0.0f, 400.0f, 600.0f, 300.0f, 1, anim_type::IDLE, entity_state::DBJUMP, {58.0f, 32.0f, 12.0f, 64.0f}};

	//anim_entity archer_anim(s_archer_data);
	//sprite_sheet archer_sprite("res/spritesheet.png", 8, 8, 128, 128);
	//
	entity_info knight_info = {200.0f, 100.0f, 0.0f, 0.0f, 400.0f, 600.0f, 300.0f, 1, anim_type::IDLE, entity_state::DBJUMP, {64.0f, 15.0f, 32.0f, 70.0f}};

	anim_entity knight_anim(s_knight_data, 10);
	sprite_sheet knight_sprite("res/HeroKnight.png", 10, 9, 160, 88);

	tile_map tmap("res/maridia_purple.png", "res/test1.map", "res/test1.col", 40, 32, 24, 32, 32, 60);
	proj_pool pool("res/projectile.png", 1, 1, 80, 10, 10);

	//trigger::g_links.add(&archer_anim, &archer_info);
	//trigger::g_links.add(&archer_anim, &pool);

	force::temp wl_jmp;

	const Uint8 *key_states = SDL_GetKeyboardState(nullptr);
	
	game_clock clk;

	while(true)
	{
		if(!cntrl::mv_cmds(key_states, cmds))
			break;

		clk.tick();

		if(knight_anim.loop_done())
			knight_info.state &= ~(entity_state::ILOCK | entity_state::MV_LEFT | entity_state::MV_RIGHT);

		if(!(knight_info.state & entity_state::ILOCK))
		{
			for(int i = 0; i < cmds.num_cmds(); i++)
			{
				if(trans::archer(knight_info, cmds.get_cmd(i)))
					break;
			}
		} 

		cmds.clear();

		if(knight_info.state & entity_state::WLJMP) {
			knight_info.state &= ~entity_state::WLJMP;
			knight_info.vel_y = -knight_info.max_y;
			knight_info.dir *= -1;
			wl_jmp.activate(knight_info.dir * 600.0f, 0.0f, 300000);
		}

		wl_jmp.apply(knight_info, clk.get_dt(), clk.get_df());

		if(knight_info.state & entity_state::MV_RIGHT) {
			knight_info.dir = 1;
			knight_info.vel_x = knight_info.acc_x;
		}

		if(knight_info.state & entity_state::MV_LEFT) {
			knight_info.dir = -1;
			knight_info.vel_x = -knight_info.acc_x;
		}

		if(!(knight_info.state & (entity_state::MV_RIGHT | entity_state::MV_LEFT)))
			knight_info.vel_x = 0.0f;

		force::gravity(knight_info, clk.get_df());

		knight_info.vel_x *= force::fric_x(knight_info, 0.5f) * std::max(coll::resolve_slope_x(knight_info, tmap, clk.get_df()), coll::resolve_x(knight_info, tmap, clk.get_df()));
		knight_info.x += knight_info.vel_x * clk.get_df();

		float res_y = coll::resolve_y(knight_info, tmap, clk.get_df());
		float res_down_slope_y = coll::resolve_slope_down_y(knight_info, tmap, clk.get_df());

		if(knight_info.dir == -1 && res_down_slope_y != 0.0f) res_y = res_down_slope_y;
		else res_y = std::min(res_y, coll::resolve_slope_y(knight_info, tmap, clk.get_df()));

		knight_info.vel_y *= force::fric_y(knight_info, 0.8f) * res_y;
		knight_info.y += knight_info.vel_y * clk.get_df();

		knight_info.vel_y *= !((knight_info.state & entity_state::GROUNDED) == entity_state::GROUNDED);

		if(!(knight_info.state & entity_state::ILOCK))
			state_anim::archer(knight_info);

		knight_anim.set_anim(anim_conv::knight(knight_info.anim));
		knight_anim.update(clk.get_dt());

		pool.update(clk.get_dt(), clk.get_df());

		glClear(GL_COLOR_BUFFER_BIT);

		tile_prog.bind();
		tmap.render(tile_prog);

		anim_prog.bind();
		knight_sprite.render(anim_prog, knight_anim.get_row(), knight_anim.get_frame(), knight_info.x, knight_info.y, knight_info.dir);
		pool.render(anim_prog);

		SDL_GL_SwapWindow(m_window);
	}
}

game::~game()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
