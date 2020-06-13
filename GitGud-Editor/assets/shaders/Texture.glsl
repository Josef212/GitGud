#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCords;

uniform mat4 u_vp;
uniform mat4 u_model;

out vec2 v_uv;

void main()
{
	v_uv = a_texCords;
	gl_Position = u_vp * u_model * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

in vec2 v_uv;

void main()
{
	color = texture(u_texture, v_uv);
}