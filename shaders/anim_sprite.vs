#version 330 core

layout (location = 0) in vec4 vert;

out vec2 tex_coord;

uniform mat4 tex_mat;
uniform mat4 mod_mat;

void main()
{
	tex_coord = (tex_mat * vec4(vert.zw, 0.0f, 1.0f)).xy;

	gl_Position = mod_mat * vec4(vert.xy, 0.0f, 1.0f);
}
