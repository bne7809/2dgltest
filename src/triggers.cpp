#include "triggers.hpp"

namespace trigger
{
	link_group g_links;

	void pink_arrow(void *from)
	{
		g_links.m_data.from = from;
		g_links.m_data.fdata1 = 600.0f;
		g_links.m_data.fdata2 = 0.0f;
		g_links.m_data.idata1 = 0;
		g_links.m_data.idata2 = 0;
		g_links.m_data.idata3 = 0;
		g_links.m_data.idata4 = 0;
		g_links.m_data.call_inst = F1VXF2VYI1RI2SI3EI4D_SEINF_FPRJCR;

		g_links.exec();
	}

	void link_group::add(void *key, void *val)
	{
		if(m_size < NUM_LINKS)
		{
			m_mem[m_size].key = key;
			m_mem[m_size].val = val;
			m_size++;
		}
	}

	void link_group::exec()
	{
		void *ptrs[5];

		int j = 0;
		
		if(m_data.call_inst & F1VXF2VYI1RI2SI3EI4D_SEINF_FPRJCR)
		{
			for(int i = 0; i < m_size; i++)
			{
				if(m_mem[i].key == m_data.from)
				{
					ptrs[j] = m_mem[i].val;
					j++;
					if(j > 1) break;
				}
			}

			entity_info *in_data = static_cast<entity_info*>(ptrs[0]);
			proj_pool *out_func = static_cast<proj_pool*>(ptrs[1]);

			out_func->create_proj(in_data->x + in_data->hit.off_x +
								(in_data->dir == 1) * in_data->hit.w / 2.0f + (in_data->dir == -1) * (-70.0f),
								in_data->y + in_data->hit.off_y + 20.0f,
								m_data.fdata1, m_data.fdata2, in_data->dir, m_data.idata1,
								m_data.idata2, m_data.idata3, m_data.idata4);
		}
	}
}
