#version 430

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// Inputs from the vertex shader
in vec3 objectColor;
in vec3 Normal;	
in vec2 TexCoords;
in vec3 FragPos;

// Uniforms

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

uniform int sampleFromDiffuse;

uniform vec3 ambColor;
uniform float ambIntensity;

uniform vec3 diffPosition;
uniform vec3 diffColor;
uniform float diffIntensity;

// Outputs
out vec4 fragColor;


void main()
{   
	// // Sample from texture or use object color
	// if(sampleFromDiffuse == 0)
	// 	fragColor = vec4(objectColor, 1.0);
	// else
	// 	fragColor = texture(material.diffuse, TexCoords);

	// Ambient light
	vec4 ambient = ambIntensity * vec4(ambColor, 1.0);

	// Diffuse light
	vec3 norm = normalize(Normal);
	vec3 diffLightDir = normalize(diffPosition - FragPos);
	float diff = max(dot(Normal, diffLightDir), 0.0); // The impact of diff light on the vertex is the dot product of the light direction and the normal
	//vec3 diffuse =  light.diffuse * (diff * diffColor);
	vec4 diffuse = vec4(0.0);
	if(sampleFromDiffuse == 0)
		diffuse = diff * vec4(objectColor, 1.0);
	else 
		diffuse = (diff * texture(material.diffuse, TexCoords));

	//Specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-diffLightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec4 specular = vec4(0.0);
	if(sampleFromDiffuse == 0)
		specular = spec * vec4(objectColor, 1.0);
	else
    	specular =  (spec * texture(material.specular, TexCoords));  

	// vec4 resultColor = vec4(ambient,1.0) + vec4(diffuse, 1.0) + fragColor ;
	vec4 resultColor = ambient + diffuse + specular;

	fragColor = resultColor;
}