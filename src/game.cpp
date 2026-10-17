#include "shader.hpp"
#include "game.hpp"
#include "anim_entity.hpp"
#include "clock.hpp"
#include "controllers.hpp"
#include "sprite_sheet.hpp"
#include "anim_conv.hpp"
#include "state_anim_conv.hpp"
#include "transformers.hpp"
#include "triggers.hpp"
#include "proj_pool.hpp"

const anim_data game::s_archer_data[] = 
{
	{0, 0, 7, 100000, 0, nullptr},
	{1, 0, 7, 200000, 1, nullptr},
	{2, 0, 6, 100000, -1, nullptr},
	{3, 0, 6, 150000, -1, nullptr},
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
	{1, 8, 6, 100000, -1, trigger::green_sword},
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
	glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_window = glfwCreateWindow(1280, 720, "Test", NULL, NULL);
	glfwMakeContextCurrent(m_window);
	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 1280, 720);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void game::start()
{
	cmd_buff cmds;

	shader anim_prog("shaders/anim_sprite.vs", "shaders/anim_sprite.fs");
	shader proj_prog("shaders/proj.vs", "shaders/anim_sprite.fs");
	
	dyn_entity knight_info;
	knight_info.state = 0;
	knight_info.x = 200.0f;
	knight_info.y = 100.0f;
	knight_info.vx = 0.0f;
	knight_info.vy = 0.0f;
	knight_info.ax = 400.0f;
	knight_info.ay = 600.0f;
	knight_info.my = 300.0f;
	knight_info.off_x = 64.0f;
	knight_info.off_y = 15.0f;
	knight_info.w = 32.0f;
	knight_info.h = 70.0f;
	knight_info.anim = anim_type::IDLE;

	anim_entity knight_anim(s_knight_data, 10);
	sprite_sheet knight_sprite("res/HeroKnight.png", 10, 9, 160, 88);

	proj_pool pool("res/proj_test.png", 8, 5, 144, 144);

	trigger::g_links.add(&knight_anim, &knight_info);
	trigger::g_links.add(&knight_anim, &pool);
	
	game_clock clk;

	while(!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();
		cntrl::mv_cmds(m_window, cmds);

		clk.tick();

		if (knight_anim.loop_done())
		{
			BIT_OFF(knight_info.state, entity_state::ILOCK);
			BIT_OFF(knight_info.state, entity_state::MV_LEFT);
			BIT_OFF(knight_info.state, entity_state::MV_RIGHT);
		}

		if (!IS_BIT(knight_info.state, entity_state::ILOCK))
		{
			for(int i = 0; i < cmds.num_cmds(); i++)
			{
				if(trans::knight(knight_info, cmds.get_cmd(i)))
					break;
			}
		}

		cmds.clear();

		if (IS_BIT(knight_info.state, entity_state::MV_RIGHT) && IS_BIT(knight_info.state, entity_state::MV_LEFT))
			knight_info.vx = 0.0f;
		else if (IS_BIT(knight_info.state, entity_state::MV_RIGHT))
			knight_info.vx = knight_info.ax;
		else if (IS_BIT(knight_info.state, entity_state::MV_LEFT))
			knight_info.vx = -knight_info.ax;
		else {
			knight_info.vx = 0.0f;
		}

		if(!IS_BIT(knight_info.state, entity_state::ILOCK))
			state_anim::knight(knight_info);

		knight_anim.set_anim(anim_conv::knight(knight_info.anim));
		knight_anim.update(clk.get_dt());

		knight_info.x += knight_info.vx * clk.get_df();

		pool.update(clk.get_dt(), clk.get_df());

		glClear(GL_COLOR_BUFFER_BIT);

		anim_prog.bind();
		int dir = !IS_BIT(knight_info.state, entity_state::X_DIR) - IS_BIT(knight_info.state, entity_state::X_DIR);
		knight_sprite.render(anim_prog, knight_anim.get_row(), knight_anim.get_frame(), knight_info.x, knight_info.y, dir);

		proj_prog.bind();
		pool.render(proj_prog);

		glfwSwapBuffers(m_window);
	}
}

game::~game()
{
	glfwTerminate();
}
