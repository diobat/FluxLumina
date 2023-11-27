#version 430
struct GSamples
{
    sampler2D position;
    sampler2D normal;
    sampler2D albedo;
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
float maxRange;
samplerCube shadowMap;
};

//Uniforms
layout(std140) uniform viewPortBlock
{
    vec2 viewPortSize;
};

// Geometric Pass data
uniform	GSamples gData;
// Point lights
uniform int numPointLights;
uniform PointLight pointLight;

// Outputs
layout (location = 0) out vec4 FragColor;

// Helper functions
vec3 calcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 albedo)
{
    // Light direction component
	vec3 lightDir = normalize(light.position - FragPos);

	// Angular Diffuse component
	float diff = max(dot(normal, lightDir), 0.0);

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Combine results
    //  Light color times diffuse component times fragment colot times attenuation
	vec3 diffuse = light.diffuse * diff * albedo * attenuation;

	return diffuse;
}

void main()
{
    vec2 screenSpace;
    screenSpace.x = gl_FragCoord.x / viewPortSize.x;
    screenSpace.y = gl_FragCoord.y / viewPortSize.y;

    // Get the Geometic Pass data
    vec3 FragPos = texture(gData.position, screenSpace).xyz;
    vec3 normal = texture(gData.normal, screenSpace).xyz;
    vec3 albedo = texture(gData.albedo, screenSpace).xyz;
    float specular = texture(gData.albedo, screenSpace).w;

    // Base lighting
    vec3 totalLight;

    totalLight += calcPointLight(pointLight, normal, FragPos, albedo);

    if(distance(FragPos, pointLight.position) > pointLight.maxRange)
    {
        totalLight = vec3(0.0);
    }

    FragColor = vec4(totalLight, 1.0);  
}