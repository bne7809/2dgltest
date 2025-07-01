#version 330 core

layout (location = 0) in vec4 vert;
layout (location = 1) in vec4 off;
layout (location = 2) in float xflip;

out vec2 tex_coord;

uniform mat4 mod_mat;

void main()
{
	vec4 tmp = vert;

	tmp.z = abs(tmp.z - xflip);

	tex_coord = tmp.zw + tmp.zw;

	gl_Position = mod_mat * vec4(tmp.xy + off.xy, 0.0f, 1.0f);
}
