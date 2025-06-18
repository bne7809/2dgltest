#include "collisions.hpp"

namespace coll
{
	float resolve_x(entity_info& out, const tile_map& cmap, float df)
	{
		const float dirx = out.vel_x * df;

		const int xleft_next = cmap.get_x(out.x + out.hit.off_x + dirx);
		const int xright_next = cmap.get_x(out.x + out.hit.off_x + out.hit.w + dirx);

		const int ytop = cmap.get_y(out.y + out.hit.off_y);
		const int ybot = cmap.get_y(out.y + out.hit.off_y + out.hit.h - 0.1f);

		float rx = 1.0f;

		float minc = std::numeric_limits<float>::infinity();
		float maxc = 0.0f;

		bool coll_x = false;

		if(out.dir == 1) {
			for(int y = ytop; y <= ybot; y++)
			{
				if(cmap.get_col(xright_next, y) == '#') {
					const float curr_minc = std::max(cmap.get_fy(y), out.y + out.hit.off_y);
					const float curr_maxc = std::min(cmap.get_fy(y + 1), out.y + out.hit.off_y + out.hit.h);

					minc = std::min(minc, curr_minc);
					maxc = std::max(maxc, curr_maxc);

					rx = (cmap.get_fx(xright_next) - out.x - out.hit.off_x - out.hit.w) / dirx;

					coll_x = true;
				}
			}
		} else {
			for(int y = ytop; y <= ybot; y++)
			{
				if(cmap.get_col(xleft_next, y) == '#') {
					const float curr_minc = std::max(cmap.get_fy(y), out.y + out.hit.off_y);
					const float curr_maxc = std::min(cmap.get_fy(y + 1), out.y + out.hit.off_y + out.hit.h);

					minc = std::min(minc, curr_minc);
					maxc = std::max(maxc, curr_maxc);

					rx = (cmap.get_fx(xleft_next + 1) - out.x - out.hit.off_x) / dirx;

					coll_x = true;
				}
			}
		}

		if(std::isnan(rx)) return 1.0f;

		if(coll_x && (maxc - minc >= out.hit.h - 10.0f)) out.state |= (entity_state::COLLX | entity_state::DBJUMP);
		else out.state &= ~entity_state::COLLX;

		return rx;
	}

	float resolve_y(entity_info& out, const tile_map& cmap, float df)
	{
		const float diry = out.vel_y * df;

		const int xleft = cmap.get_x(out.x + out.hit.off_x + 0.1f);
		const int xright = cmap.get_x(out.x + out.hit.off_x + out.hit.w - 0.1f);

		const int ytop_next = cmap.get_y(out.y + out.hit.off_y + diry);
		const int ybot_next = cmap.get_y(out.y + out.hit.off_y + out.hit.h + diry);

		float ry = 1.0f;

		if(diry < 0.0f) {
			for(int x = xleft; x <= xright; x++)
			{
				if(cmap.get_col(x, ytop_next) == '#') {
					ry = (cmap.get_fy(ytop_next + 1) - out.y - out.hit.off_y) / diry;
					break;
				}
			}
		} else {
			for(int x = xleft; x <= xright; x++)
			{
				if(cmap.get_col(x, ybot_next) == '#') {
					ry = (cmap.get_fy(ybot_next) - out.y - out.hit.off_y - out.hit.h) / diry;
					out.state |= (entity_state::GROUNDED | entity_state::DBJUMP | entity_state::JMP);
					break;
				}
			}
		}

		if(std::isnan(ry)) return 1.0f;

		if(ry == 1.0f) out.state &= ~entity_state::GROUNDED;

		return ry;
	}
}
