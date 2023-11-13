#version 430

struct Material{
sampler2D diffuse;
sampler2D specular;
float shininess;
sampler2D normal;
};

struct DirLight{
vec3 direction;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

struct PointLight{
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
float constant;
float linear;
float quadratic;
float farPlane;
samplerCube shadowMap;
};

struct SpotLight{
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
sampler2D shadowMap;
};

// Inputs from the vertex shader
in VertexOutput{
	vec3 objectColor;
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
} FragmentIn;

in LightSpaceVertexOutput{
	vec4 Spotlight[10];
} LightSpaceFragmentIn;

//////////////////////////
// Uniforms
//////////////////////////

// Textures
uniform Material material;
uniform int sampleFromDiffuse;
uniform int sampleFromSpecular;
uniform int sampleFromNormal;

// Directional lights
uniform int numDirLights;
uniform DirLight dirLight[3];
// Point lights
uniform int numPointLights;
uniform PointLight pointLight[10];
// Spot lights
uniform int numSpotLights;
uniform SpotLight spotLight[10];

layout(std140) uniform viewPosBlock
{
	vec3 viewPos;
};

layout(std140) uniform shadowSettingsBlock
{
	bool directional;
	bool point;
	bool spot;
};

layout(std140) uniform normalMappingBlock
{
	int normalMapping;
};

// Outputs
out vec4 fragColor;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// Helper functions
float PointLightShadowCalculation(int index, vec3 fragPos)
{
	// get vector between fragment position and light position
    vec3 fragToLight = fragPos - pointLight[index].position;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / pointLight[index].farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointLight[index].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= pointLight[index].farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}

float SpotLightShadowCalculation(int index, vec4 posLightSpace, vec3 normal, vec3 lightDir)
{
	// Perspective division
	vec3 projCoords = posLightSpace.xyz / posLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(spotLight[index].shadowMap, projCoords.xy).r;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
 	// Remove shadow acne by adding a bias
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 
	// Check whether current frag pos is in shadow
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

	return shadow;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 diffTex = vec3(0.0);
	vec3 specTex = vec3(0.0);

	if(sampleFromDiffuse == 1)
		diffTex = vec3(texture(material.diffuse, FragmentIn.TexCoords));
	else
		diffTex = FragmentIn.objectColor;

	if(sampleFromSpecular == 1)
		specTex = vec3(texture(material.specular, FragmentIn.TexCoords));
	else
		specTex = FragmentIn.objectColor;

	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light.ambient * diffTex;
	vec3 diffuse = light.diffuse * diff * diffTex;
	vec3 specular = light.specular * spec * specTex;
	return (ambient + diffuse + specular);
}

vec3 calcPointLight(int i, PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
	vec3 diffTex = vec3(0.0);
	vec3 specTex = vec3(0.0);

	if(sampleFromDiffuse == 1)
		diffTex = vec3(texture(material.diffuse, FragmentIn.TexCoords));
	else
		diffTex = FragmentIn.objectColor;

	if(sampleFromSpecular == 1)
		specTex = vec3(texture(material.specular, FragmentIn.TexCoords));
	else
		specTex = FragmentIn.objectColor;

	vec3 lightDir = normalize(light.position - FragPos);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
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

	// Calculate shadow
	float shadow = 0.0;
	if(point)
	{
		shadow = PointLightShadowCalculation(i, FragmentIn.FragPos);
	}
	return ( (1.0 - shadow) * (diffuse + specular));
}

vec3 calcSpotLight(int i, SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{

	vec3 diffTex = vec3(0.0);
	vec3 specTex = vec3(0.0);

	if(sampleFromDiffuse == 1)
		diffTex = vec3(texture(material.diffuse, FragmentIn.TexCoords));
	else
		diffTex = FragmentIn.objectColor;

	if(sampleFromSpecular == 1)
		specTex = vec3(texture(material.specular, FragmentIn.TexCoords));
	else
		specTex = FragmentIn.objectColor;

	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
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
	
	// Calculate shadow
	float shadow = 0.0;
	if(spot)
	{
		shadow = SpotLightShadowCalculation(i, LightSpaceFragmentIn.Spotlight[i], normal, lightDir);
	}
	return ( (1.0 - shadow) * (diffuse + specular) );
}

void main()
{
	vec3 totalLight = vec3(0.0);

	// Calculate common input arguments for all the light calculations
	vec3 norm;
	vec3 viewDir = normalize(viewPos - FragmentIn.FragPos);
	
	if(sampleFromNormal == 0)
	{
		norm = normalize(FragmentIn.Normal);
	}
	else
	{
		norm = texture(material.normal, FragmentIn.TexCoords).rgb;
		norm = norm * 2.0 - 1.0;   
		norm = normalize(FragmentIn.TBN * norm); 
	}


	// 1 - Directional Lights
	for(int i = 0; i < numDirLights; i++)
	{
		totalLight += calcDirLight(dirLight[i], norm, viewDir);
	}
	// 2 - Point Lights
	for(int i = 0; i < numPointLights; i++)
	{
		totalLight += calcPointLight(i , pointLight[i], norm, FragmentIn.FragPos, viewDir);
	}
	// 3 - Spot light
	for(int i = 0; i < numSpotLights; i++)
	{	
		totalLight += calcSpotLight(i, spotLight[i], norm, FragmentIn.FragPos, viewDir);
	}

	fragColor = vec4(totalLight, 1.0);
}
