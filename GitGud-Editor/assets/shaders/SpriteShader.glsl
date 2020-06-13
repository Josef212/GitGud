#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCords;
layout(location = 3) in float a_texIndex;
layout(location = 4) in vec2 a_tiling;

uniform mat4 u_vp;

out vec4 v_color;
out vec2 v_uv;
out float v_texIndex;
out vec2 v_tiling;

void main()
{
	v_color = a_color;
	v_uv = a_texCords;
	v_texIndex = a_texIndex;
	v_tiling = a_tiling;

	gl_Position = u_vp * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_textures[32];

in vec4 v_color;
in vec2 v_uv;
in float v_texIndex;
in vec2 v_tiling;

void main()
{	
	color = texture(u_textures[int(v_texIndex)], v_uv * v_tiling) * v_color;
}