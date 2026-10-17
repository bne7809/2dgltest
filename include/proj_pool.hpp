#pragma once

#include "BlockAllocator.hpp"
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct proj_data
{
	float x;
	float y;
	float vx;
	float vy;
	int time;
	int row;
	int frame;
	int start;
	int end;
	int delay;
};

constexpr size_t CPC = 64;
constexpr size_t NC = 2;

class proj_pool : public BlockAllocator<proj_data, CPC, NC>
{
private:
	struct inst_data
	{
		float x;
		float y;
		float u;
		float v;
		float flip;
	};

	static_assert(sizeof(inst_data) == 5 * sizeof(float));

	size_t num_inst;

	GLuint m_tex;
	GLuint m_vao;
	GLuint m_vbo[2];

	const float m_fracw;
	const float m_frach;
public:
	proj_pool(const char *file, int num_x, int num_y, int rwidth, int rheight);

	void create_proj(float x, float y, float vx, float vy, int row, int start, int end, int delay);
	void update(int dt, float df);
	void render(shader &s);

	~proj_pool();
};
