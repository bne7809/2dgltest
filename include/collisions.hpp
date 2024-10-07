#pragma once

#include "tile_map.hpp"
#include "entity_info.hpp"
#include <vector>

namespace coll
{
	float resolve_slope_x(entity_info& out, const tile_map& cmap, float df);
	float resolve_slope_down_y(entity_info& out, const tile_map& cmap, float df);
	float resolve_slope_y(entity_info& out, const tile_map& cmap, float df);
	float resolve_x(entity_info& out, const tile_map& cmap, float df);
	float resolve_y(entity_info& out, const tile_map& cmap, float df);
	void resolve(entity_info& out, const tile_map& cmap, float df);
}
