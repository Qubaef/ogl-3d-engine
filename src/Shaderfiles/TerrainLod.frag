#version 430 core

//
// Data structures
//
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

//
// Uniforms
//
uniform vec3 viewPos;
uniform Material material;
uniform DirectionalLight dir_light;

uniform float nodeSize;
uniform sampler2D terrainHeight;

//
// Ouputs
//
out vec4 fragColor;

//
// Inputs
//
in vec3 normal;
in vec3 fragPos;

in vec2 gs_terrainTexCoord;


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

void main()
{
	//// Calculate result basing on directional light
	vec3 view_dir = normalize(viewPos - fragPos);
	vec4 color = vec4(calculate_dir_light(dir_light, normal, view_dir), 1.f);

	//vec4 color = vec4(mix(0.0, 1.0, nodeSize / 1000.0), mix(1.0, 0.0, nodeSize / 1000.0), 0.0, 1.0);
	// vec4 color = texture(terrainHeight, gs_terrainTexCoord);
	fragColor = color;
}