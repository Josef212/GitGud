#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCords;
layout(location = 3) in float a_texIndex;
layout(location = 4) in vec2 a_tiling;
layout(location = 5) in int a_entityId;

uniform mat4 u_vp;

out vec4 v_color;
out vec2 v_uv;
out flat float v_texIndex;
out vec2 v_tiling;
flat out int v_entityId;

void main()
{
	v_color = a_color;
	v_uv = a_texCords;
	v_texIndex = a_texIndex;
	v_tiling = a_tiling;
	v_entityId = a_entityId;

	gl_Position = u_vp * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityId;

uniform sampler2D u_textures[32];

in vec4 v_color;
in vec2 v_uv;
in flat float v_texIndex;
in vec2 v_tiling;
flat in int v_entityId;

void main()
{
	vec4 texColor = v_color;
	switch(int(v_texIndex))
	{
		case 0: texColor *= texture(u_textures[0], v_uv * v_tiling); break;
		case 1: texColor *= texture(u_textures[1], v_uv * v_tiling); break;
		case 2: texColor *= texture(u_textures[2], v_uv * v_tiling); break;
		case 3: texColor *= texture(u_textures[3], v_uv * v_tiling); break;
		case 4: texColor *= texture(u_textures[4], v_uv * v_tiling); break;
		case 5: texColor *= texture(u_textures[5], v_uv * v_tiling); break;
		case 6: texColor *= texture(u_textures[6], v_uv * v_tiling); break;
		case 7: texColor *= texture(u_textures[7], v_uv * v_tiling); break;
		case 8: texColor *= texture(u_textures[8], v_uv * v_tiling); break;
		case 9: texColor *= texture(u_textures[9], v_uv * v_tiling); break;
		case 10: texColor *= texture(u_textures[10], v_uv * v_tiling); break;
		case 11: texColor *= texture(u_textures[11], v_uv * v_tiling); break;
		case 12: texColor *= texture(u_textures[12], v_uv * v_tiling); break;
		case 13: texColor *= texture(u_textures[13], v_uv * v_tiling); break;
		case 14: texColor *= texture(u_textures[14], v_uv * v_tiling); break;
		case 15: texColor *= texture(u_textures[15], v_uv * v_tiling); break;
		case 16: texColor *= texture(u_textures[16], v_uv * v_tiling); break;
		case 17: texColor *= texture(u_textures[17], v_uv * v_tiling); break;
		case 18: texColor *= texture(u_textures[18], v_uv * v_tiling); break;
		case 19: texColor *= texture(u_textures[19], v_uv * v_tiling); break;
		case 20: texColor *= texture(u_textures[20], v_uv * v_tiling); break;
		case 21: texColor *= texture(u_textures[21], v_uv * v_tiling); break;
		case 22: texColor *= texture(u_textures[22], v_uv * v_tiling); break;
		case 23: texColor *= texture(u_textures[23], v_uv * v_tiling); break;
		case 24: texColor *= texture(u_textures[24], v_uv * v_tiling); break;
		case 25: texColor *= texture(u_textures[25], v_uv * v_tiling); break;
		case 26: texColor *= texture(u_textures[26], v_uv * v_tiling); break;
		case 27: texColor *= texture(u_textures[27], v_uv * v_tiling); break;
		case 28: texColor *= texture(u_textures[28], v_uv * v_tiling); break;
		case 29: texColor *= texture(u_textures[29], v_uv * v_tiling); break;
		case 30: texColor *= texture(u_textures[30], v_uv * v_tiling); break;
		case 31: texColor *= texture(u_textures[31], v_uv * v_tiling); break;
	}

	color = texColor;
	entityId = v_entityId;
}