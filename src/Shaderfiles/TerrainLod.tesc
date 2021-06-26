#version 430

//
// Uniforms
//
uniform mat4 matMvp;
uniform mat4 matMv;
uniform mat4 matM;
uniform mat4 matP;

uniform vec3 nodePos;
uniform float nodeSize;

uniform vec2 viewport;
uniform sampler2D terrainHeight;
uniform float terrainHeightOffset;

uniform float tscaleNegx;
uniform float tscaleNegz;
uniform float tscalePosx;
uniform float tscalePosz;

//
// Inputs
//
in vec2 vs_terrainTexCoord[];

in vec3 normal[];
in vec3 fragPos[];

//
// Outputs
//
layout(vertices = 4) out;

patch out float gl_TessLevelOuter[4];
patch out float gl_TessLevelInner[2];

out vec2 tcs_terrainTexCoord[];
out float tcs_tessLevel[];

out vec3 normal[];
out vec3 fragPos[];

/**
* Dynamic level of detail using camera distance algorithm.
*/
float dlodCameraDistance(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	vec4 samp = texture(terrainHeight, t0);
	p0.y = samp[0] * terrainHeightOffset;
	samp = texture(terrainHeight, t1);
	p1.y = samp[0] * terrainHeightOffset;

	vec4 view0 = matMv * p0;
	vec4 view1 = matMv * p1;

	float MinDepth = 10.0;
	float MaxDepth = 100000.0;

	float d0 = clamp( (abs(p0.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
	float d1 = clamp( (abs(p1.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);

	float t = mix(64, 2, (d0 + d1) * 0.5);

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

/**
* Dynamic level of detail using sphere algorithm.
* Source adapated from the DirectX 11 Terrain Tessellation example.
*/
float dlodSphere(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	float g_tessellatedTriWidth = 10.0;

	vec4 samp = texture(terrainHeight, t0);
	p0.y = samp[0] * terrainHeightOffset;
	samp = texture(terrainHeight, t1);
	p1.y = samp[0] * terrainHeightOffset;

	vec4 center = 0.5 * (p0 + p1);
	vec4 view0 = matMv * center;
	vec4 view1 = view0;
	view1.x += distance(p0, p1);

	vec4 clip0 = matP * view0;
	vec4 clip1 = matP * view1;

	clip0 /= clip0.w;
	clip1 /= clip1.w;

	//clip0.xy *= Viewport;
	//clip1.xy *= Viewport;

	vec2 screen0 = ((clip0.xy + 1.0) / 2.0) * viewport;
	vec2 screen1 = ((clip1.xy + 1.0) / 2.0) * viewport;
	float d = distance(screen0, screen1);

	// g_tessellatedTriWidth is desired pixels per tri edge
	float t = clamp(d / g_tessellatedTriWidth, 0,64);

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

/*float dlodSphere(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	float g_tessellatedTriWidth = 10.0;

	vec4 samp = texture(terrainHeight, t0);
	p0.y = samp[0] * terrainHeightOffset;
	samp = texture(terrainHeight, t1);
	p1.y = samp[0] * terrainHeightOffset;

	vec4 center = 0.5 * (p0 + p1);
	float radius = distance(p0, p1) / 2;

	vec4 sc0 = matMv * p0;
	vec4 sc1 = sc0;
	sc0.x -= radius;
	sc1.x += radius;
	
	vec4 clip0 = pMatrix * sc0;
	vec4 clip1 = pMatrix * sc1;

	clip0 /= clip0.w;
	clip1 /= clip1.w;

	clip0.xy *= Viewport;
	clip1.xy *= Viewport;

	float d = distance(clip0, clip1);

	// g_tessellatedTriWidth is desired pixels per tri edge
	float t = clamp(d / g_tessellatedTriWidth, 0,64);

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
}*/

void main(void)
{
	// Outer tessellation level
	gl_TessLevelOuter[0] = dlodCameraDistance(gl_in[3].gl_Position, gl_in[0].gl_Position, tcs_terrainTexCoord[3], tcs_terrainTexCoord[0]);
	gl_TessLevelOuter[1] = dlodCameraDistance(gl_in[0].gl_Position, gl_in[1].gl_Position, tcs_terrainTexCoord[0], tcs_terrainTexCoord[1]);
	gl_TessLevelOuter[2] = dlodCameraDistance(gl_in[1].gl_Position, gl_in[2].gl_Position, tcs_terrainTexCoord[1], tcs_terrainTexCoord[2]);
	gl_TessLevelOuter[3] = dlodCameraDistance(gl_in[2].gl_Position, gl_in[3].gl_Position, tcs_terrainTexCoord[2], tcs_terrainTexCoord[3]);
	
	if (tscaleNegx == 2.0)
		gl_TessLevelOuter[0] = max(2.0, gl_TessLevelOuter[0] * 0.5);
	if (tscaleNegz == 2.0)
		gl_TessLevelOuter[1] = max(2.0, gl_TessLevelOuter[1] * 0.5);
	if (tscalePosx == 2.0)
		gl_TessLevelOuter[2] = max(2.0, gl_TessLevelOuter[2] * 0.5);
	if (tscalePosz == 2.0)
		gl_TessLevelOuter[3] = max(2.0, gl_TessLevelOuter[3] * 0.5);

	// Inner tessellation level
	gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[3]);
	gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[2] + gl_TessLevelOuter[1]);

	// Pass the patch verts along
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// Output heightmap coordinates
	tcs_terrainTexCoord[gl_InvocationID] = vs_terrainTexCoord[gl_InvocationID];

	// Output tessellation level (used for wireframe coloring)
	tcs_tessLevel[gl_InvocationID] = gl_TessLevelOuter[0];
}