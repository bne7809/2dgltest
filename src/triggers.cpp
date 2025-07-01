#include "triggers.hpp"
#include "entity_info.hpp"
#include "proj_pool.hpp"

namespace trigger
{
	link_group g_links;

	void green_sword(void *from)
	{
		g_links.m_data.from = from;

		g_links.m_data.fdata1 = 600.0f;
		g_links.m_data.fdata2 = 0.0f;
		g_links.m_data.idata1 = 4;
		g_links.m_data.idata2 = 0;
		g_links.m_data.idata3 = 7;
		g_links.m_data.idata4 = 100;
		g_links.m_data.call_inst = F1VXF2VYI1RI2SI3EI4D_SEINF_FPRJCR;

		g_links.exec();
	}

	void link_group::add(void *k, void *v)
	{
		if(m_size < NUM_LINKS)
		{
			key[m_size] = k;
			val[m_size] = v;
			++m_size;
		}
	}

	void link_group::exec()
	{
		if(m_data.call_inst & F1VXF2VYI1RI2SI3EI4D_SEINF_FPRJCR)
		{
			int j = 0;

			dyn_entity *in_data;
			proj_pool *out_func;

			for(int i = 0; i < m_size; i++)
			{
				if(key[i] == m_data.from)
				{
					if(j == 0)
						in_data = static_cast<dyn_entity*>(val[i]);
					else
						out_func = static_cast<proj_pool*>(val[i]);
					++j;
					if(j > 1) break;
				}
			}

			int dir = !IS_BIT(in_data->state, entity_state::X_DIR) - IS_BIT(in_data->state, entity_state::X_DIR);

			out_func->create_proj(in_data->x + in_data->off_x +
								(dir == 1) * in_data->w / 2.0f + (dir == -1) * (-70.0f),
								in_data->y + in_data->off_y + 20.0f,
							    dir * m_data.fdata1, m_data.fdata2, m_data.idata1,
								m_data.idata2, m_data.idata3, m_data.idata4);
		}
	}
}
