#version 430 core

//// Output value
out vec4 frag_final_color;

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

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//// Input from vertex shader
in vec3 normal;
in vec3 frag_pos;

//// Uniforms
uniform vec3 view_pos;
uniform Material material;

// lights
uniform DirectionalLight dir_light;

#define MAX_POINT_LIGHTS_COUNT 16
uniform int point_lights_count;
uniform PointLight point_lights[MAX_POINT_LIGHTS_COUNT];

vec3 calculate_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir);
vec3 calculate_point_lights(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main()
{
	vec3 view_dir = normalize(view_pos - frag_pos);

	//// Calculate result for every light type

	// Directional light
	vec3 all_lights_result = calculate_dir_light(dir_light, normal, view_dir);

	// Point lights
	for(int i = 0; i < point_lights_count; i++) 
	{
		all_lights_result += calculate_point_lights(point_lights[i], normal, frag_pos, view_dir);
	}

	//// Final result
	frag_final_color = vec4(all_lights_result, 1.f);
}


vec3 calculate_dir_light(DirectionalLight light, vec3 normal, vec3 view_dir)
{
	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	// Diffuse
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normalize(normal), normalize(light.direction)), 0.0) ;
	// Specular
	vec3 reflect_dir = reflect(normalize(-light.direction), normalize(normal));
	vec3 specular = light.specular * material.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	return (ambient + diffuse + specular);
}


vec3 calculate_point_lights(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
	vec3 light_dir = normalize(light.position - frag_pos);

	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	// Diffuse
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normalize(normal), light_dir), 0.0) ;
	// Specular
	vec3 reflect_dir = reflect(-light_dir, normalize(normal));
	vec3 specular = light.specular * material.specular * pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	// Calculate power of the light
	float distance = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return (ambient + diffuse + specular) * attenuation;
}