#version 430

//
// Uniforms
//
uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform vec3 viewPos;
uniform vec3 nodePos;
uniform float nodeSize;

uniform vec2 viewport;
uniform sampler2D terrainHeight;
uniform float terrainDensity;
uniform float terrainSize;
uniform vec3 terrainOffset;

//
// Inputs
//
in vec3 vert_fragPos[];

//
// Outputs
//
layout(vertices = 4) out;

//patch out float gl_TessLevelOuter[4];
//patch out float gl_TessLevelInner[2];

out float tesc_tessLevel[];

///**
//* Dynamic level of detail using camera distance algorithm.
//*/
//float dlodCameraDistance(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
//{
//	vec4 samp = texture(terrainHeight, t0);
//	p0.y = samp[0] + terrainHeightOffset;
//	samp = texture(terrainHeight, t1);
//	p1.y = samp[0] + terrainHeightOffset;
//
//	// float MinDepth = 10.0;
//	// float MaxDepth = 100000.0;
//
//	// float d0 = clamp( (abs(p0.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
//	// float d1 = clamp( (abs(p1.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
//
//	float d0 = distance(vec4(viewPos, 1), p0);
//	float d1 = distance(vec4(viewPos, 1), p1);
//
//	float t = mix(64, 2, (d0 + d1) * 0.5);
//
//	if (t <= 2.0)
//	{ 
//		return 2.0;
//	}
//	if (t <= 4.0)
//	{ 
//		return 4.0;
//	}
//	if (t <= 8.0)
//	{ 
//		return 8.0;
//	}
//	if (t <= 16.0)
//	{ 
//		return 16.0;
//	}
//	if (t <= 32.0)
//	{ 
//		return 32.0;
//	}
//	
//	return 64.0;
//}
//
///**
//* Dynamic level of detail using sphere algorithm.
//* Source adapated from the DirectX 11 Terrain Tessellation example.
//*/
//float dlodSphere(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
//{
//	float g_tessellatedTriWidth = 10.0;
//
//	vec4 samp = texture(terrainHeight, t0);
//	p0.y = samp[0] + terrainHeightOffset;
//	samp = texture(terrainHeight, t1);
//	p1.y = samp[0] + terrainHeightOffset;
//
//	vec4 center = 0.5 * (p0 + p1);
//	vec4 view0 = matMv * center;
//	vec4 view1 = view0;
//	view1.x += distance(p0, p1);
//
//	vec4 clip0 = matP * view0;
//	vec4 clip1 = matP * view1;
//
//	clip0 /= clip0.w;
//	clip1 /= clip1.w;
//
//	//clip0.xy *= Viewport;
//	//clip1.xy *= Viewport;
//
//	vec2 screen0 = ((clip0.xy + 1.0) / 2.0) * viewport;
//	vec2 screen1 = ((clip1.xy + 1.0) / 2.0) * viewport;
//	float d = distance(screen0, screen1);
//
//	// g_tessellatedTriWidth is desired pixels per tri edge
//	float t = clamp(d / g_tessellatedTriWidth, 0, 64);
//
//	if (t <= 2.0)
//	{ 
//		return 2.0;
//	}
//	if (t <= 4.0)
//	{ 
//		return 4.0;
//	}
//	if (t <= 8.0)
//	{ 
//		return 8.0;
//	}
//	if (t <= 16.0)
//	{ 
//		return 16.0;
//	}
//	if (t <= 32.0)
//	{ 
//		return 32.0;
//	}
//	
//	return 64.0;
//}

float tessLevelDist(vec3 cameraPos, int coordId1, int coordId2)
{
	vec4 p1 = gl_in[coordId1].gl_Position;
	vec4 p2 = gl_in[coordId2].gl_Position;
	p1.y = texture(terrainHeight, vec2(p1.x, p1.z) / terrainSize + 0.5).x + terrainOffset.y;
	p2.y = texture(terrainHeight, vec2(p2.x, p2.z) / terrainSize + 0.5).x + terrainOffset.y;
	float dist1 = distance(cameraPos, vec3(p1));
	float dist2 = distance(cameraPos, vec3(p2));

	float avgDistance = (dist1 + dist2) * 0.5;

	if (avgDistance <= 16.0)
	{
		return 64.0;
	}
	if (avgDistance <= 32.0)
	{
		return 32.0;
	}
	if (avgDistance <= 64.0)
	{
		return 16.0;
	}
	if (avgDistance <= 128.0)
	{
		return 8.0;
	}
	if (avgDistance <= 512.0)
	{
		return 4.0;
	}

	return 2.0;
}

float tessLevelSphere(int coordId1, int coordId2)
{
	float triangleTargetWidth = 7.0;

	vec4 p1 = gl_in[coordId1].gl_Position;
	vec4 p2 = gl_in[coordId2].gl_Position;

	p1.y = texture(terrainHeight, vec2(p1.x, p1.z) / terrainSize + 0.5).x + terrainOffset.y;
	p2.y = texture(terrainHeight, vec2(p2.x, p2.z) / terrainSize + 0.5).x + terrainOffset.y;

	vec4 center = 0.5 * (p1 + p2);
	vec4 view1 = matMv * center;
	vec4 view2 = view1;
	view2.x += distance(p1, p2);

	vec4 clip1 = matP * view1;
	vec4 clip2 = matP * view2;

	clip1 /= clip1.w;
	clip2 /= clip2.w;

	vec2 screen1 = ((clip1.xy + 1.0) / 2.0) * viewport;
	vec2 screen2 = ((clip2.xy + 1.0) / 2.0) * viewport;

	float diameter = distance(screen1, screen2);
	float t = clamp(floor(diameter / triangleTargetWidth), 0, 64);

	if (t <= 2.0)
	{
		return 2.0;
	}
	if (t <= 4.0)
	{
		return 4.0;
	}
	if (t <= 8.0)
	{
		return 8.0;
	}
	if (t <= 16.0)
	{
		return 16.0;
	}
	if (t <= 32.0)
	{
		return 32.0;
	}

	return 64.0;
}

float getDistance(int coordId1, int coordId2)
{
	float triangleTargetWidth = 7.0;

	vec4 p1 = gl_in[coordId1].gl_Position;
	vec4 p2 = gl_in[coordId2].gl_Position;

	p1.y = texture(terrainHeight, vec2(p1.x, p1.z) / terrainSize).x + terrainOffset.y;
	p2.y = texture(terrainHeight, vec2(p2.x, p2.z) / terrainSize).x + terrainOffset.y;

	return distance(p1, p2);
}

// Translate world space to screen space
vec2 worldToScreen(vec4 p)
{
	p = matP * p;
	p /= p.w;
	p.xy *= viewport;
	p.xy += viewport * 0.5;
	return p.xy;
}

// Calculate the distance of two world space points in screen space
float screenDistance(vec4 p1, vec4 p2)
{
	p1 = matP * p1;
	p1 /= p1.w;
	p2 = matP * p2;
	p2 /= p2.w;
	p1.xy *= viewport;
	p2.xy *= viewport;
	p1.xy += viewport * 0.5;
	p2.xy += viewport * 0.5;
	return distance(p1.xy, p2.xy);
}

void main(void)
{
	// Outer tessellation level
	// gl_TessLevelOuter[0] = tessLevelDist(viewPos, 0, 1);
	// gl_TessLevelOuter[1] = tessLevelDist(viewPos, 1, 3);
	// gl_TessLevelOuter[2] = tessLevelDist(viewPos, 2, 0);
	// gl_TessLevelOuter[3] = tessLevelDist(viewPos, 3, 2);

	gl_TessLevelOuter[0] = tessLevelSphere(0, 2);
	gl_TessLevelOuter[1] = tessLevelSphere(0, 1);
	gl_TessLevelOuter[2] = tessLevelSphere(1, 3);
	gl_TessLevelOuter[3] = tessLevelSphere(2, 3);

	// Inner tessellation level
	gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]);
	gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]);

	// Pass the patch verts along
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}