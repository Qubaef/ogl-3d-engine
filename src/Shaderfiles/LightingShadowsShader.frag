#version 430 core

//// Output value
out vec4 FragColor;

//// Data structures
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

//// Input from vertex shader
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

//// Uniforms
layout (std140, binding = 0) uniform GlobalData
{
	// Display mode
	int displayMode;

	// Camera and viewport
	vec3 viewPos;
	vec3 viewDir;
	float nearPlane;
	float farPlane;

	mat4 MVP;
	mat4 M;
	mat4 V;
	mat4 P;

	// Lights
	vec3 dirLightAmbient;
	vec3 dirLightDiffuse;
	vec3 dirLightSpecular;
	vec3 dirLightDirection;
} g;

layout (std140, binding = 1) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};

layout (std140, binding = 2) uniform Cascades
{
    float cascadePlaneDistances[16];
    float maxShadowCascadeBias[16];
    int cascadeCount;
};

layout (std140, binding = 3) uniform ShadowsInfo
{
    float shadowStrength;
};

uniform Material material;
uniform sampler2DArray shadowMap;

// Gloval variables
vec3 layer1Color = vec3(1.0, 0.73, 0.0);

// Array of colors for each layer
# define LAYER_DEBUG_COLORS 7
vec3 layerDebugColor[] = {
    vec3(0.98, 0.25, 0.27),
    vec3(0.95, 0.45, 0.17),
    vec3(0.97, 0.59, 0.12),
    vec3(0.98, 0.78, 0.31),
    vec3(0.56, 0.75, 0.43),
    vec3(0.26, 0.67, 0.55),
    vec3(0.34, 0.46, 0.56)
};

//// Functions declarations
int bitTest(int value, int bit);
float getLinearDepth(float zNear, float zFar);
vec3 calculateDirLight(vec3 normal, vec3 view_dir, Material material);
float calculateShadows(vec3 fragPosWorldSpace);
int calculateLayer(vec3 fragPosWorldSpace);

void main() {
	vec3 view_dir = normalize(g.viewPos - fs_in.FragPos);
    vec3 all_lights_result;

	// Calculate result for every light type
    if(bitTest(g.displayMode, 4) == 1) {
        Material debugMaterial;
        int layer = calculateLayer(fs_in.FragPos);

        if(layer < LAYER_DEBUG_COLORS) {
            debugMaterial.ambient = layerDebugColor[layer] * shadowStrength;
            debugMaterial.diffuse = layerDebugColor[layer];
        } else {
            debugMaterial.ambient = vec3(0.0);
            debugMaterial.diffuse = vec3(0.0);
        }

        debugMaterial.specular = vec3(0.0);
        debugMaterial.shininess = 0;

        // Directional light
	    all_lights_result = calculateDirLight(fs_in.Normal, view_dir, debugMaterial);
    }
    else {
        // Directional light
	    all_lights_result = calculateDirLight(fs_in.Normal, view_dir, material);
    }

	// Final result
	FragColor = vec4(all_lights_result, 1.f);
}

int bitTest(int value, int bit){
    return (value >> bit) & 1;
}

float getLinearDepth(float zNear, float zFar)
{
    float z_n = 2.0 * gl_FragCoord.z - 1.0;
     return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

vec3 calculateDirLight(vec3 normal, vec3 view_dir, Material material)
{
	// Ambient
	vec3 ambient = g.dirLightAmbient * material.ambient;
	// Diffuse
	vec3 diffuse = g.dirLightDiffuse * material.diffuse * max(dot(normalize(normal), normalize(g.dirLightDirection)), 0.0);
	// Specular
	vec3 reflect_dir = reflect(normalize(-g.dirLightDirection), normalize(normal));
	vec3 specular = g.dirLightSpecular * material.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Calculate shadow
	float shadowComponent = calculateShadows(fs_in.FragPos);

	return (ambient + (1.0 - shadowComponent) * (diffuse + specular));
}

float calculateShadows(vec3 fragPosWorldSpace)
{
    int layer = calculateLayer(fragPosWorldSpace);

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(fragPosWorldSpace, 1.0);
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    if (currentDepth  > 1.0) {
        return 0.0;
    }

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.05 * (1.0 - dot(normal, g.dirLightDirection)), maxShadowCascadeBias[layer]);
    if (layer == cascadeCount) {
        bias *= 1 / (g.farPlane * 0.5f);
    }
    else {
        bias *= 1 / (cascadePlaneDistances[layer] * 0.5f);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    const int halfkernelWidth = 3;
    for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x) {
    	for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y) {
    		float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
    		shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;
    	}
    }
    shadow /= ((halfkernelWidth * 2 + 1) * (halfkernelWidth * 2 + 1));

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow * shadowStrength;
}

int calculateLayer(vec3 fragPosWorldSpace) {
    // select cascade layer
    vec4 fragPosViewSpace = g.V * vec4(fragPosWorldSpace, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i) {
        if (depthValue < cascadePlaneDistances[i]){
            layer = i;
            break;
        }
    }
    if (layer == -1) {
        layer = cascadeCount;
    }

    return layer;
}