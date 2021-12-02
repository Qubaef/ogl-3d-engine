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
uniform Material materialFlat;
uniform Material materialSteep;
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
in float steepness;

in vec3 tese_normal;
in vec3 tese_fragPos;

Material interpolateMaterials(Material m1, Material m2, float val) {
	Material newM;
	newM.ambient = mix(m1.ambient, m2.ambient, val);
	newM.diffuse = mix(m1.diffuse, m2.diffuse, val);
	newM.specular = mix(m1.specular, m2.specular, val);
	newM.shininess = m1.shininess * val + m2.shininess * (1 - val);

	return newM;
}


vec3 calculate_dirLight(DirectionalLight light, vec3 normal, vec3 viewDir, Material material)
{
	// Ambient
	vec3 ambient = light.ambient * material.ambient;
	// Diffuse
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normalize(normal), normalize(light.direction)), 0.0) ;
	// Specular
	vec3 reflect_dir = reflect(normalize(-light.direction), normalize(normal));
	vec3 specular = light.specular * material.specular * pow(max(dot(viewDir, reflect_dir), 0.0), material.shininess);

	return (ambient + diffuse + specular);
}

#define LOG2 1.442695

void main()
{
	//// Calculate result basing on directional light
	vec3 viewDir = normalize(viewPos - tese_fragPos);
	// gl_FragDepth;
	vec4 color = vec4(calculate_dirLight(dir_light, tese_normal, viewDir, interpolateMaterials(materialSteep, materialFlat, steepness)), 1.f);
	
	float u_fogDensity = 0.00010;
	float fogDistance = length(viewPos - tese_fragPos);
	float fogAmount = 1. - exp2(-u_fogDensity * u_fogDensity * fogDistance * fogDistance * LOG2);
	vec4 fogColor = vec4(0.86, 0.9, 1.0, 1.0);

	fragColor = mix(color, fogColor, fogAmount);

	// fragColor = color;
}