#pragma once

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

		int m_size;

		void *key[NUM_LINKS];
		void *val[NUM_LINKS];

	};

	extern link_group g_links;

	void green_sword(void *from);
}
