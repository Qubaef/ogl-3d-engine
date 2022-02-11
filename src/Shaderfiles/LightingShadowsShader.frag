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

//// Functions declarations
float getLinearDepth(float zNear, float zFar);
vec3 calculateDirLight(vec3 normal, vec3 view_dir);
float calculateShadows(vec3 fragPosWorldSpace);
int calculateLayer(vec3 fragPosWorldSpace);

void main()
{
	vec3 view_dir = normalize(g.viewPos - fs_in.FragPos);
    vec3 all_lights_result;

	//// Calculate result for every light type

    if(g.displayMode == 4) {
        vec3 debugColor = vec3(0.0);
        int layer = calculateLayer(fs_in.FragPos);

        if(layer == 0) {
            debugColor = vec3(0.02, 0.84, 0.63);
        }
        else if(layer == 1){
            debugColor = vec3(1.0, 0.82, 0.4);
        }
        else if(layer == 2){
            debugColor = vec3(0.94, 0.28, 0.44);
        }
        else if(layer == 3){
            debugColor = vec3(0.07, 0.54, 0.70);
        }
        else if(layer == 4){
            debugColor = vec3(0.03, 0.23, 0.30);
        }
        else{
            debugColor = vec3(0.0);
        }

        // Get depth of the fragment
        debugColor *= getLinearDepth(g.nearPlane, g.farPlane) * 0.01;

        FragColor = vec4(debugColor, 1.0);
        return;
    }
    else {
        // Directional light
	    all_lights_result = calculateDirLight(fs_in.Normal, view_dir);
    }

	//// Final result
	FragColor = vec4(all_lights_result, 1.f);
}

float getLinearDepth(float zNear, float zFar)
{
    float z_n = 2.0 * gl_FragCoord.z - 1.0;
     return 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));
}

vec3 calculateDirLight(vec3 normal, vec3 view_dir)
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