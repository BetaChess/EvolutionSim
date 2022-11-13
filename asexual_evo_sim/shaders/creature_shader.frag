#version 450

layout (location = 0) in vec2 fragOffset;

layout (location = 0) out vec4 outColor;


layout(set = 0, binding = 0) uniform GlobalUbo 
{
	mat4 projection;
} ubo;

layout(push_constant) uniform Push
{
	vec4 position;
	mat4 transform;
	vec3 color;
} push;

// X, Y, RADIUS
const vec3 CIRCLEDEFS[3] = vec3[](
	vec3(-0.2, 0, 0.2),
	vec3(0.2, 0, 0.2),
	vec3(0.675, 0, 0.275)
);

const int NUMOFTRIANGLES = 4;
const vec2 TRIANGLES[NUMOFTRIANGLES * 3] = vec2[](
	vec2(0.12, 0.2), vec2(0.28, 0.2), vec2(0.2, 0.38),
	vec2(-0.12, 0.2), vec2(-0.28, 0.2), vec2(-0.2, 0.38),
	vec2(0.12, -0.2), vec2(0.28, -0.2), vec2(0.2, -0.38),
	vec2(-0.12, -0.2), vec2(-0.28, -0.2), vec2(-0.2, -0.38)
);

const vec2 EYES[2 * 3] = vec2[](
	vec2(0.75, 0.045), vec2(0.835, 0.045), vec2(0.75, 0.175),
	vec2(0.75, -0.045), vec2(0.835, -0.045), vec2(0.75, -0.175)
);

float sgn(vec2 p1, vec2 p2, vec2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool PointInTriangle (vec2 pt, vec2 v1, vec2 v2, vec2 v3)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sgn(pt, v1, v2);
    d2 = sgn(pt, v2, v3);
    d3 = sgn(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void main()
{
	bool inside = false;
	bool insideEyes = false;

	for (int i = 0; i < CIRCLEDEFS.length(); i++)
	{
		vec2 diff = vec2(CIRCLEDEFS[i].x, CIRCLEDEFS[i].y) - fragOffset;
	
		inside = inside || (dot(diff, diff) < CIRCLEDEFS[i].z * CIRCLEDEFS[i].z);
	}

	for (int i = 0; i < TRIANGLES.length(); i += 3)
	{
		inside = inside || PointInTriangle(fragOffset, TRIANGLES[i + 0], TRIANGLES[i + 1], TRIANGLES[i + 2]);
	}

	for (int i = 0; i < EYES.length(); i += 3)
	{
		insideEyes = insideEyes || PointInTriangle(fragOffset, EYES[i + 0], EYES[i + 1], EYES[i + 2]);
	}

	outColor = vec4(
	float(!insideEyes) * float(inside) * push.color + 
	float(insideEyes) * vec3(0.65, 0.05, 0.1), 
	float(inside) * 1.0);
}

