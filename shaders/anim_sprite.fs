#version 330 core

in vec2 tex_coord;

out vec4 col;

uniform sampler2D image;

void main()
{
	col = texture(image, tex_coord);
}
