#include "proj_pool.hpp"
#include "utils.hpp"

proj_pool::proj_pool(const char *file, int num_x, int num_y, int rwidth, int rheight)
	: num_inst(0), m_fracw(1.0f/num_x), m_frach(1.0f/num_y)
{
	util::tex_create(file, &m_tex);

	const float width = static_cast<float>(rwidth);
	const float height = static_cast<float>(rheight);

	float vertices[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		width, 0.0f, m_fracw, 0.0f,
		0.0f, height, 0.0f, m_frach,

		width, 0.0f, m_fracw, 0.0f,
		width, height, m_fracw, m_frach,
		0.0f, height, 0.0f, m_frach
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(2, m_vbo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(inst_data) * CPC * NC, nullptr, GL_STREAM_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribDivisor(2, 1);

	glBindVertexArray(0);
}

void proj_pool::create_proj(float x, float y, float vx, float vy, int row, int start, int end, int delay)
{
	proj_data *p = alloc();

	p->x = x;
	p->y = y;
	p->vx = vx;
	p->vy = vy;
	p->time = 0;
	p->row = row;
	p->frame = start;
	p->start = start;
	p->end = end;
	p->delay = delay;
}

void proj_pool::update(int dt, float df)
{
	size_t i = 0;

	const float fracw = m_fracw;
	const float frach = m_frach;

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);

	inst_data *ptr = static_cast<inst_data*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(inst_data) * CPC * NC, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));

	update_del_if([dt, df](proj_data &data)
			{

				data.x += data.vx * df;
				data.y += data.vy * df;

				if(data.time > data.delay)
				{
					data.frame++;
					if(data.frame > data.end)
						data.frame = data.start;
					data.time = 0;
				}
				else
					data.time += dt;
			},
			[ptr, &i, fracw, frach](proj_data &data)
			{
				if(data.x < 0.0f || data.x >= 1280.0f)
					return true;
				if(data.y < 0.0f || data.y >= 720.0f)
					return true;

				ptr[i].x = data.x;
				ptr[i].y = data.y;
				ptr[i].u = data.frame * fracw;
				ptr[i].v = data.row * frach;
				ptr[i].flip = fracw * (data.vx < 0.0f);

				++i;

				return false;
			});

	glUnmapBuffer(GL_ARRAY_BUFFER);

	num_inst = i;
}

void proj_pool::render(shader& s)
{
	glm::mat4 mod_mat = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f);

	s.set_mat4("mod_mat", glm::value_ptr(mod_mat));

	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_tex);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, num_inst);
}

proj_pool::~proj_pool()
{
	glDeleteTextures(1, &m_tex);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(2, m_vbo);
}
