// Renderer 2D Circle Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_worldPosition;
layout(location = 1) in vec3 a_localPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_entityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_vp;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityId;

void main()
{
	Output.LocalPosition = a_localPosition;
	Output.Color = a_color;
	Output.Thickness = a_thickness;
	Output.Fade = a_fade;

	v_EntityId = a_entityId;

	gl_Position = u_vp * vec4(a_worldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityId;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityId;

void main()
{
	float distance = 1.0 - length(Input.LocalPosition);
	float circle = smoothstep(0.0, Input.Fade, distance);
	circle *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (circle == 0.0)
	{
		discard;
	}

	o_color = Input.Color;
	o_color.a *= circle;

	o_entityId = v_EntityId;
}