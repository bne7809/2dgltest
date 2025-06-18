#include "utils.hpp"

namespace util
{
	void tex_create(const char *img, GLuint *tex)
	{
		SDL_Surface *img_surf = IMG_Load(img);

		glGenTextures(1, tex);
		glBindTexture(GL_TEXTURE_2D, *tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_surf->w, img_surf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, img_surf->pixels);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		SDL_FreeSurface(img_surf);
	}
}
