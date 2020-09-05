#version 430 core

out vec4 frag_final_color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 frag_pos;
in vec3 fragment_color;

uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main(){

	// Calculate 3 main light components to get final color of the fragment

	// Ambient
	vec3 ambient_light = light.ambient * material.ambient;
	// Diffuse
	vec3 diffuse_light = light.diffuse * material.diffuse * max(dot(normalize(normal), normalize(light.direction)), 0.0) ;

	// Specular
	vec3 view_direction = normalize(view_pos - frag_pos);
	vec3 reflect_dir = reflect(normalize(light.direction), normalize(normal));
	vec3 specular_light = light.specular * material.specular * pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);

	// Final
	vec3 color_val = (ambient_light + diffuse_light + specular_light) * fragment_color;

	frag_final_color = vec4(color_val, 1.f);
}