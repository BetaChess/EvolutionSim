#version 450

const vec2 OFFSETS[6] = vec2[](
	vec2(-1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, -1.0),
	vec2(1.0, -1.0),
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout(push_constant) uniform Push
{
	vec4 position;
	mat4 transform;
	vec3 color;
} push;

layout(set = 0, binding = 0) uniform GlobalUbo 
{
	mat4 projection;
} ubo;

void main()
{
	fragOffset = OFFSETS[gl_VertexIndex];

	gl_Position = ubo.projection * push.transform * vec4(OFFSETS[gl_VertexIndex], 0.0, 1.0) + push.position;
}

