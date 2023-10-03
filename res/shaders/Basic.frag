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
float innerCutOff;
float outerCutOff;

vec3 ambient;
vec3 diffuse;
vec3 specular;

float constant;
float linear;
float quadratic;
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
// Directional lights
uniform int numDirLights;
uniform DirLight dirLight[3];
// Spot lights
uniform int numSpotLights;
uniform SpotLight spotLight[20];

uniform vec3 viewPos;
uniform vec3 ambColor;
uniform float ambIntensity;

// Outputs
out vec4 fragColor;

// Helper functions
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
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


vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
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

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
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

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

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
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	return (ambient + diffuse + specular);

}

void main()
{
	vec3 totalLight = vec3(0.0);

	// Calculate common input arguments for all the light calculations
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	// 1 - Directional Lights
	for(int i = 0; i < numDirLights; i++)
	{
		totalLight += calcDirLight(dirLight[i], norm, viewDir);
	}
	// 2 - Point Lights
	for(int i = 0; i < numPointLights; i++)
	{
		totalLight += calcPointLight(pointLight[i], norm, FragPos, viewDir);
	}
	// 3 - Spot light
	for(int i = 0; i < numSpotLights; i++)
	{
		totalLight += calcSpotLight(spotLight[i], norm, FragPos, viewDir);
	}

	fragColor = vec4(totalLight, 1.0);
}
