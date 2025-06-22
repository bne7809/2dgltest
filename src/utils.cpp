#include "utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace util
{
	void tex_create(const char *img, GLuint *tex)
	{
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(0);

		unsigned char *data = stbi_load(img, &width, &height, &nrChannels, 0);
		if (!data) {
			fprintf(stderr, "Failed to load texture: %s\n", img);
			return ;
		}

		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glGenTextures(1, tex);
		glBindTexture(GL_TEXTURE_2D, *tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
}
