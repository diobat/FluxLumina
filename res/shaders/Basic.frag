#version 430

struct Material
{
sampler2D diffuse;
sampler2D specular;
float shininess;
};

struct DirLight
{
vec3 direction;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct PointLight
{
vec3 position;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;
};

struct SpotLight
{
vec3 position;

vec3 direction;

float cutOff;
float outerCutOff;
};

// Inputs from the vertex shader
in vec3 objectColor;
in vec3 Normal;	
in vec2 TexCoords;
in vec3 FragPos;

//////////////////////////
// Uniforms
//////////////////////////

// Textures
uniform Material material;
uniform int sampleFromDiffuse;
uniform int sampleFromSpecular;

// Point lights
uniform int numPointLights;
uniform PointLight pointLight[20];
uniform int numDirLights;
uniform DirLight dirLight[3];

// Directional lights
uniform vec3 viewPos;

uniform vec3 ambColor;
uniform float ambIntensity;

// Outputs
out vec4 fragColor;

// Helper functions
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 diffTex = vec3(0.0);
	vec3 specTex = vec3(0.0);

	if(sampleFromDiffuse == 1)
		diffTex = vec3(texture(material.diffuse, TexCoords));
	else
		diffTex = objectColor;

	if(sampleFromSpecular == 1)
		specTex = vec3(texture(material.specular, TexCoords));
	else
		specTex = objectColor;

	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light.ambient * diffTex;
	vec3 diffuse = light.diffuse * diff * diffTex;
	vec3 specular = light.specular * spec * specTex;
	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 diffTex = vec3(0.0);
	vec3 specTex = vec3(0.0);

	if(sampleFromDiffuse == 1)
		diffTex = vec3(texture(material.diffuse, TexCoords));
	else
		diffTex = objectColor;

	if(sampleFromSpecular == 1)
		specTex = vec3(texture(material.specular, TexCoords));
	else
		specTex = objectColor;

	vec3 lightDir = normalize(light.position - FragPos);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// Combine results
	vec3 ambient = light.ambient * diffTex;
	vec3 diffuse = light.diffuse * diff * diffTex;
	vec3 specular = light.specular * spec * specTex;
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

// vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
// {
// 	vec3 lightDir = normalize(light.position - FragPos);
// 	// Diffuse shading
// 	float diff = max(dot(normal, lightDir), 0.0);
// 	// Specular shading
// 	vec3 reflectDir = reflect(-lightDir, normal);
// 	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
// 	// Attenuation
// 	float distance = length(light.position - FragPos);
// 	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
// 	// Combine results
// 	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
// 	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
// 	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
// 	ambient *= attenuation;
// 	diffuse *= attenuation;
// 	specular *= attenuation;
// 	return (ambient + diffuse + specular);
// }

void main()
{
	vec3 totalLight = vec3(0.0);


	// Calculate common input arguments for all the light calculations
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// 1 - Directional Lights
	for(int i = 0; i < numDirLights; i++)
	{
		totalLight += CalcDirLight(dirLight[i], norm, viewDir);
	}

	// 2 - Point Lights
	for(int i = 0; i < numPointLights; i++)
	{
		totalLight += CalcPointLight(pointLight[i], norm, FragPos, viewDir);
	}

	// 3 - Spot light
	// totalLight += calcSpotLight(spotLight, norm, FragPos, viewDir);



	fragColor = vec4(totalLight, 1.0);
}




// void main()
// {   
// // Attenuation
// float distance = length(light.position - FragPos);
// float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

// // Ambient light
// vec4 ambient = vec4(0.0);
// if(sampleFromDiffuse == 0)
// 	vec4 ambient = ambIntensity * vec4(light.ambient, 1.0) * vec4(objectColor,1.0);
// else 
// 	vec4 ambient = ambIntensity * vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);

// // Diffuse light
// vec3 norm = normalize(Normal);
// vec3 diffLightDir = normalize(light.position - FragPos);
// float diff = max(dot(Normal, diffLightDir), 0.0); // The impact of diff light on the vertex is the dot product of the light direction and the normal
// vec4 diffuse = vec4(0.0);
// if(sampleFromDiffuse == 0)
// 	diffuse = diff * vec4(objectColor, 1.0);
// else 
// 	diffuse = vec4(light.diffuse, 1.0) * (diff * texture(material.diffuse, TexCoords));

// //Specular light
// vec3 viewDir = normalize(viewPos - FragPos);
// vec3 reflectDir = reflect(-diffLightDir, norm);  
// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
// vec4 specular = vec4(0.0);
// if(sampleFromSpecular == 0)
// 	;
// else
// 	specular = vec4(light.specular, 1.0) * (spec * texture(material.specular, TexCoords));  

// fragColor = ambient +  (diffuse + specular) * attenuation;
// }