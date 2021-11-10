#version 430 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform vec2 viewport;
uniform float farPlane;           // Far plane  
uniform float focusPoint;         // Far plane  
uniform float focusScale;         // Far plane  

/* Default blur */
const float offset = 1.0 / 300.0;  

vec4 blurColor() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);
}


/* Bokeh depth of field - Dennis Gustafsson */
const float GOLDEN_ANGLE = 2.39996323;
const float MAX_BLUR_SIZE = 20.0;
const float RAD_SCALE = 0.5; // Smaller = nicer blur, larger = faster
const vec2 PixelSize = vec2(1.0) / viewport;

float getBlurSize(float depth, float focusPoint, float focusScale)
{
	float coc = clamp((1.0 / focusPoint - 1.0 / depth)*focusScale, -1.0, 1.0);
	return abs(coc) * MAX_BLUR_SIZE;
}

vec3 depthOfField(vec2 texCoord, float focusPoint, float focusScale)
{
	float centerDepth = texture(depthTexture, texCoord).r * farPlane;
	float centerSize = getBlurSize(centerDepth, focusPoint, focusScale);
	vec3 color = texture(screenTexture, texCoord).rgb;
	float tot = 1.0;
	float radius = RAD_SCALE;
	for (float ang = 0.0; radius<MAX_BLUR_SIZE; ang += GOLDEN_ANGLE)
	{
		vec2 tc = texCoord + vec2(cos(ang), sin(ang)) * PixelSize * radius;
		vec3 sampleColor = texture(screenTexture, tc).rgb;
		float sampleDepth = texture(depthTexture, tc).r * farPlane;
		float sampleSize = getBlurSize(sampleDepth, focusPoint, focusScale);
		if (sampleDepth > centerDepth)
			sampleSize = clamp(sampleSize, 0.0, centerSize*2.0);
		float m = smoothstep(radius-0.5, radius+0.5, sampleSize);
		color += mix(color/tot, sampleColor, m);
		tot += 1.0;   radius += RAD_SCALE/radius;
	}
	return color /= tot;
}


// Sepia
vec3 sepia(vec3 rgb) {
    return vec3(0.393 * rgb.r + 0.769 * rgb.g + 0.189 * rgb.b,
                0.349 * rgb.r + 0.686 * rgb.g + 0.168 * rgb.b,
                0.272 * rgb.r + 0.534 * rgb.g + 0.131 * rgb.b);
}

// Antialiasing - Fast approximate anti-aliasing
vec4 antiAlias(vec2 texCoord) {
    vec4 color = vec4(0.0);
    color += texture(screenTexture, texCoord + vec2(-0.5, 0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(0.5, 0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(-0.5, -0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(0.5, -0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(0.0, 0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(0.0, -0.5) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(0.5, 0.0) * PixelSize);
    color += texture(screenTexture, texCoord + vec2(-0.5, 0.0) * PixelSize);
    return color / 8.0;
}


// Pixelate
vec4 pixelate(vec2 texCoord, int pixelSize) {
    vec2 pixelCoord = floor(texCoord * vec2(pixelSize));
    return texture(screenTexture, pixelCoord / vec2(pixelSize));
}

void main()
{
    /* Standard output */
    // FragColor = texture(screenTexture, TexCoords);

    // float depth = texture(depthTexture, TexCoords).r * farPlane;
    // FragColor = vec4(depth, depth, depth, 1.0);

    /* Depth basic test */
    // vec4 depth = texture(depthTexture, TexCoords);
    // 
    // if(depth.r != 1){
    //     depth.r = 0.0;
    // }
    // 
    // FragColor = vec4(depth.r, depth.r, depth.r, 1.0);

    /* Depth off field */
    // vec2 texCoord = TexCoords;
    // FragColor = vec4(depthOfField(TexCoords, focusPoint, focusScale), 1.0);

    // Apply sepia
    // FragColor = vec4(sepia(vec3(texture(screenTexture, TexCoords))), 1.0);


    // Apply anit-aliasing
    FragColor = antiAlias(TexCoords);

    // Pixelate
    // FragColor = pixelate(TexCoords, 300);

    // Apply rain effect
    // FragColor = rainDrops(TexCoords, 100);
}