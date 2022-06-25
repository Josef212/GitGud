// Renderer 2D Circle Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in int a_entityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_vp;
};

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) out VertexOutput Output;
layout(location = 1) out flat int v_EntityId;

void main()
{
	Output.Color = a_color;
	v_EntityId = a_entityId;

	gl_Position = u_vp * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityId;

struct VertexOutput
{
	vec4 Color;
};

layout(location = 0) in VertexOutput Input;
layout(location = 1) in flat int v_EntityId;

void main()
{
	o_color = Input.Color;
	o_entityId = v_EntityId;
}