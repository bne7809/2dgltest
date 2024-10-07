#version 330 core

layout (location = 0) in vec4 vert;
layout (location = 1) in vec4 off;

out vec2 tex_coord;

uniform mat4 mod_mat;

void main()
{
	tex_coord = vert.zw + off.zw;

	gl_Position = mod_mat * vec4(vert.xy + off.xy, 0.0f, 1.0f);
}
