#pragma once

#include "proj_pool.hpp"
#include "entity_info.hpp"

namespace trigger
{
	constexpr unsigned int F1VXF2VYI1RI2SI3EI4D_SEINF_FPRJCR = 0x0001;

	struct trig_data
	{
		void *from;
		float fdata1;
		float fdata2;
		int idata1;
		int idata2;
		int idata3;
		int idata4;
		unsigned int call_inst;
	};

	class link_group
	{
	public:
		void add(void *key, void *val);
		void exec();

		trig_data m_data;
	private:
		static constexpr int NUM_LINKS = 10;

		struct map
		{
			void *key;
			void *val;
		};

		map m_mem[NUM_LINKS];
		int m_size;
	};

	extern link_group g_links;

	void pink_arrow(void *from);
}
