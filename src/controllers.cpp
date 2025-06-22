#include "controllers.hpp"

namespace cntrl
{
	void mv_cmds(GLFWwindow* window, cmd_buff& buff)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

		auto keyA = glfwGetKey(window, GLFW_KEY_A);
		if (keyA == GLFW_PRESS)
			buff.push(entity_cmd::MV_LEFT);
		else if (keyA == GLFW_RELEASE)
			buff.push(entity_cmd::ST_LEFT);

		auto keyD = glfwGetKey(window, GLFW_KEY_D);
		if (keyD == GLFW_PRESS)
			buff.push(entity_cmd::MV_RIGHT);
		else if (keyD == GLFW_RELEASE)
			buff.push(entity_cmd::ST_RIGHT);

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			buff.push(entity_cmd::MV_JUMP);
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			buff.push(entity_cmd::TR_FIRE);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			buff.push(entity_cmd::MV_ROLL);
	}
}
