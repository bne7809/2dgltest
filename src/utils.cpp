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

	std::pair<tile_offset*, GLsizei> tile_off_pdest_sz(const char *map, float width, float height, float twidth, float theight, int nbytes)
	{
		GLsizei tile_count = 0;

		unsigned char *temp_buff = new unsigned char[nbytes + 10];
		
		std::ifstream map_file(map);
		map_file.read((char*) &temp_buff[0], nbytes);
		map_file.close();

		for(int i = 0; i < nbytes; i++)
		{
			if(temp_buff[i] == 0xFF || temp_buff[i] == 0xFE || temp_buff[i] == 0xFD)
			{
				i += 5;

				tile_count += (temp_buff[i] - temp_buff[i - 1] + 1);
			}
		}

		tile_offset *off_data = new tile_offset[tile_count];

		map_data c;
		int miter = 0;

		for(int i = 0; i < nbytes; i++)
		{
			if(temp_buff[i] == 0xFF || temp_buff[i] == 0xFE || temp_buff[i] == 0xFD)
			{
				c.d1 = temp_buff[++i];
				c.d2 = temp_buff[++i];
				c.d3 = temp_buff[++i];
				c.d4 = temp_buff[++i];
				c.d5 = temp_buff[++i];

				if(temp_buff[i - 5] == 0xFF)
				{
					while(c.d4 <= c.d5)
					{
						off_data[miter].x = c.d4 * width;
						off_data[miter].y = c.d1 * height;

						off_data[miter].tx = c.d2 * twidth;
						off_data[miter].ty = c.d3 * theight;
	   
						c.d4++;
						miter++;
					}
				} else if(temp_buff[i - 5] == 0xFE) {
					while(c.d4 <= c.d5)
					{
						off_data[miter].x = c.d2 * width;
						off_data[miter].y = c.d1 * height;

						off_data[miter].tx = c.d4 * twidth;
						off_data[miter].ty = c.d3 * theight;

						c.d2++;
						c.d4++;
						miter++;
					}
				} else if(temp_buff[i - 5] == 0xFD) {
					while(c.d4 <= c.d5)
					{
						off_data[miter].x = c.d1 * width;
						off_data[miter].y = c.d4 * height;

						off_data[miter].tx = c.d2 * twidth;
						off_data[miter].ty = c.d3 * theight;

						c.d4++;
						miter++;
					}
				}
			}
		}

		delete[] temp_buff;

		return std::make_pair(off_data, tile_count);
	}
}
