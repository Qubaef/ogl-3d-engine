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

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//// Input from vertex shader
in vec3 normal;
in vec3 frag_pos;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} fs_in;

//// Uniforms
uniform vec3 view_pos;
uniform Material material;
uniform sampler2DShadow shadowMap;

// lights
uniform DirectionalLight dirLight;

vec3 calculate_dirLight(DirectionalLight light, vec3 normal, vec3 view_dir);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, vec3(projCoords.xy, 0)).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(dirLight.direction - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    const int halfkernelWidth = 3;
    for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x)
    {
        for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, 0)).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= ((halfkernelWidth*2+1)*(halfkernelWidth*2+1));

    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
	vec3 view_dir = normalize(view_pos - frag_pos);

	//// Calculate result for every light type

	// Directional light
	vec3 all_lights_result = calculate_dirLight(dirLight, normal, view_dir);

	//// Final result
	FragColor = vec4(all_lights_result, 1.f);
}


vec3 calculate_dirLight(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	// Diffuse
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normalize(normal), normalize(light.direction)), 0.0) ;
	// Specular
	vec3 reflect_dir = reflect(normalize(-light.direction), normalize(normal));
	vec3 specular = light.specular * material.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Calculate shadow
	float shadowComponent = ShadowCalculation(fs_in.FragPosLightSpace);

	return (ambient + (1.0 - shadowComponent) * (diffuse + specular));
}