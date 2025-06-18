#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#define GL_GLEXT_PROTOTYPES
#include <SDL2/SDL_opengl.h>

#include "tile_info.hpp"

namespace util
{
	void tex_create(const char *img, GLuint *tex);
};
