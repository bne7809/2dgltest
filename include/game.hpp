#pragma once

#include "entity_info.hpp"
#include <GLFW/glfw3.h>

class game
{
public:
	game();
	void start();
	~game();
private:
	static const anim_data s_archer_data[9]; 
	static const anim_data s_knight_data[14];

	GLFWwindow *m_window;
};
