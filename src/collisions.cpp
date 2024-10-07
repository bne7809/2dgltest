#include "collisions.hpp"

namespace coll
{
	struct cbinf_t
	{
		int y;
		float nx;
		float ny;
		float ct;
	};

	float resolve_slope_x(entity_info& out, const tile_map& cmap, float df)
	{
		const float dirx = out.vel_x * df;

		const float rsx = out.x + out.hit.off_x + out.hit.w / 2.0f + dirx;
		const float rsy =  out.y + out.hit.off_y + out.hit.h;

		const char type = cmap.get_col(rsx, rsy);

		if(type == '1' || type == '2' || type == '3') return 1.0f;

		return 0.0f;
	}

	float resolve_slope_down_y(entity_info& out, const tile_map& cmap, float df)
	{
		if(!(out.state & entity_state::GROUNDED)) return 0.0f;

		const float diry = out.vel_y * df;

		const float rsx = out.x + out.hit.off_x + out.hit.w / 2.0f;
		const float rsy =  out.y + out.hit.off_y + out.hit.h;

		const char type = cmap.get_col(rsx, rsy);

		const float tile_x = cmap.get_fx(cmap.get_x(rsx));
		float tile_y_bot;

		if(type == '1' || type == '2' || type == '3') {
			tile_y_bot = cmap.get_fy(cmap.get_y(rsy + diry) + 1);
		} else {
			return 0.0f;
		}

		float ry = 0.0f;

		if(type == '1') {
			ry = (tile_y_bot - 0.5f * (rsx - tile_x) - rsy) / diry;
		} else if(type == '3') {
			ry = (tile_y_bot - 0.5f * (rsx - tile_x) - cmap.get_fy(1) / 2.0f - rsy) / diry;
		} else if(type == '2') {
			ry = (tile_y_bot - (rsx - tile_x) - rsy) / diry;
		}

		//std::cout << "ry: " << ry << " vel_y: " << out.vel_y << " type: " << type << "\n";

		//out.state |= (entity_state::GROUNDED | entity_state::DBJUMP | entity_state::JMP);
		//return ry;
		
		return ry;
	}

	float resolve_slope_y(entity_info& out, const tile_map& cmap, float df)
	{
		const float diry = out.vel_y * df;

		if(diry <= 0.0f) return 1.0f;

		const float rsx = out.x + out.hit.off_x + out.hit.w / 2.0f;
		const float rsy =  out.y + out.hit.off_y + out.hit.h;

		const float tile_x = cmap.get_fx(cmap.get_x(rsx));
		float tile_y_bot;

		const char type = cmap.get_col(rsx, rsy - 0.1f);

		if(type == '1' || type == '2' || type == '3') {
			tile_y_bot = cmap.get_fy(cmap.get_y(rsy - 0.1f) + 1);
		} else {
			return 1.0f;
		}

		float ry = 1.0f;

		float slope_y;

		if(type == '1') {
			slope_y = tile_y_bot - 0.5f * (rsx - tile_x);
		} else if(type == '3') {
			slope_y = tile_y_bot - 0.5f * (rsx - tile_x) - cmap.get_fy(1) / 2.0f;
		} else if(type == '2') {
			slope_y = tile_y_bot - (rsx - tile_x);
		} else {
			return 1.0f;
		}

		ry = (slope_y - rsy) / diry;

		//std::cout << "ry: " << ry << " vel_y: " << out.vel_y << " type: " << type << "\n";

		//out.state |= (entity_state::GROUNDED | entity_state::DBJUMP | entity_state::JMP);
		//return ry;

		if(ry < 1.0f) {
			out.state |= (entity_state::GROUNDED | entity_state::DBJUMP | entity_state::JMP);
			return ry;
		} else {
			out.state &= ~entity_state::GROUNDED;
			return 1.0f;
		}
	}

	float resolve_x(entity_info& out, const tile_map& cmap, float df)
	{
		const float dirx = out.vel_x * df;

		const int xleft_next = cmap.get_x(out.x + out.hit.off_x + dirx);
		const int xright_next = cmap.get_x(out.x + out.hit.off_x + out.hit.w + dirx);

		const int ytop = cmap.get_y(out.y + out.hit.off_y);
		const int ybot = cmap.get_y(out.y + out.hit.off_y + out.hit.h - 0.1f);

		//std::cout << "y: " << out.y << " ytop: " << ytop << " ybot: " << ybot << " xleft_next: " << xleft_next << " xright_next: " << xright_next << "\n";

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

	void resolve(entity_info& out, const tile_map& cmap, float df)
	{
		if(out.vel_x == 0.0f && out.vel_y == 0.0f) return;

		int x1, x2;
		int y1, y2;

		const float rsx = out.x + out.hit.off_x + out.hit.w / 2.0f;
		const float rsy =  out.y + out.hit.off_y + out.hit.h / 2.0f;

		const float dirx = out.vel_x * df;
		const float diry = out.vel_y * df;

		std::vector<cbinf_t> cblocks;

		if(out.dir == 1) {
			x1 = cmap.get_x(out.x + out.hit.off_x);
			x2 = cmap.get_x(out.x + out.hit.off_x + out.hit.w + dirx);
		} else {
			x1 = cmap.get_x(out.x + out.hit.off_x + dirx);
			x2 = cmap.get_x(out.x + out.hit.off_x + out.hit.w);
		}

		if(out.vel_y <= 0.0f) {
			y1 = cmap.get_y(out.y + out.hit.off_y + diry);
			y2 = cmap.get_y(out.y + out.hit.off_y + out.hit.h);
		} else {
			y1 = cmap.get_y(out.y + out.hit.off_y);
			y2 = cmap.get_y(out.y + out.hit.off_y + out.hit.h + diry);
		}

		cblocks.reserve((x2 - x1 + 1) * (y2 - y1 + 1));

		for(int y = y1; y <= y2; y++)
		{
			for(int x = x1; x <= x2; x++)
			{
				if(cmap.get_col(x, y) == '#') {
					float tnear_x = (cmap.get_fx(x) - out.hit.w / 2.0f - rsx) / dirx;
					float tnear_y = (cmap.get_fy(y) - out.hit.h / 2.0f - rsy) / diry;

					float tfar_x = (cmap.get_fx(x + 1) + out.hit.w / 2.0f - rsx) / dirx;
					float tfar_y = (cmap.get_fy(y + 1) + out.hit.h / 2.0f - rsy) / diry;

					if(std::isnan(tnear_x) || std::isnan(tnear_y) || std::isnan(tfar_x) || std::isnan(tfar_y))
						continue;

					if(tnear_x > tfar_x) std::swap(tnear_x, tfar_x);
					if(tnear_y > tfar_y) std::swap(tnear_y, tfar_y);

					if(tnear_x > tfar_y || tnear_y > tfar_x) continue;

					if(std::min(tfar_x, tfar_y) < 0) continue;

					const float contact_time = std::max(tnear_x, tnear_y);

					if(contact_time >= 1.0f || contact_time < 0.0f) continue;

					float norm_x, norm_y;

					if(tnear_x > tnear_y) {
						norm_x = 1.0f;
						norm_y = 0.0f;

						norm_x *= ((dirx < 0.0f) - (dirx >= 0.0f));
					} else {
						norm_x = 0.0f;
						norm_y = 1.0f;

						norm_y *= ((diry < 0.0f) - (diry >= 0.0f));
					}

					const char above = cmap.get_col(x, y - 1);

					if(norm_y == -1.0f && (above == '#')) continue;

					//std::cout << "x: " << x << " y: " << y << " cpx: " << rsx + contact_time * dirx << " cpy: " << rsy + contact_time * diry << " normx: " << norm_x << " normy: " << norm_y << " rsx: " << rsx << " rsy: " << rsy << " dirx: " << dirx << " diry: " << diry <<  " tnearx: " << tnear_x << " tneary: " << tnear_y << "\n";
					cblocks.push_back({y, norm_x, norm_y, contact_time});
				}
			}
		}

		std::sort(cblocks.begin(), cblocks.end(), [](const cbinf_t& a, const cbinf_t& b) { return a.ct < b.ct; });

		bool on_ground = false;
		bool coll_x = false;

		float minc = std::numeric_limits<float>::infinity();
		float maxc = 0.0f;

		for(auto& b : cblocks)
		{
			out.vel_x += b.nx * std::fabs(out.vel_x) * (1.0f - b.ct);
			out.vel_y += b.ny * std::fabs(out.vel_y) * (1.0f - b.ct);

			if(b.nx != 0.0f) {
				const float curr_minc = std::max(cmap.get_fy(b.y), out.y + out.hit.off_y);
				const float curr_maxc = std::min(cmap.get_fy(b.y + 1), out.y + out.hit.off_y + out.hit.h);

				if(curr_minc < minc) minc = curr_minc;
				if(curr_maxc > maxc) maxc = curr_maxc;

				coll_x = true;
			}

			if(b.ny == -1.0f) on_ground = true;
		}

		if(on_ground) out.state |= (entity_state::GROUNDED | entity_state::DBJUMP | entity_state::JMP);
		else out.state &= ~entity_state::GROUNDED;

		if(coll_x && (maxc - minc >= out.hit.h - 10.0f)) out.state |= (entity_state::COLLX | entity_state::DBJUMP);
		else out.state &= ~entity_state::COLLX;
	}
}
