#version 430

struct Material
{
	sampler2D diffuse;
	//vec3 specular;
	//float shininess;
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

uniform int sampleFromTexture;
uniform sampler2D texture0;

uniform vec3 ambColor;
uniform float ambIntensity;

uniform vec3 diffDirection;
uniform vec3 diffColor;
uniform float diffIntensity;

// Outputs
out vec4 fragColor;


void main()
{   
	// Sample from texture or use object color
	if(sampleFromTexture == 0)
		fragColor = vec4(objectColor, 1.0);
	else
		fragColor = texture(texture0, TexCoords);


	// Ambient light
	vec3 ambient = ambIntensity * ambColor;

	vec3 nuv = viewPos;
	// // Diffuse light
	// vec3 norm = normalize(Normal);
	// vec3 diffLightDir = normalize(diffDirection);
	// float diff = max(dot(Normal, diffLightDir), 0.0); // The impact of diff light on the vertex is the dot product of the light direction and the normal
	// vec3 diffuse =  light.diffuse * (diff * vec(texture(material.diffuse, TexCoords)))));


	// //Specular light
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-diffLightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = lightColor * (spec * material.specular);  

	// vec4 resultColor = vec4(ambient,1.0) + vec4(diffuse, 1.0) + fragColor ;
	vec4 resultColor = vec4(ambient,1.0) + fragColor;

	fragColor = resultColor;
}