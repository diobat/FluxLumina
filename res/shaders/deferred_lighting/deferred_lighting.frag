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
samplerCube shadowMap;
};

// Inputs
in VertexOutput
{
    vec2 TexCoords;
} FragmentIn;

// Uniforms
// Geometric Pass data
uniform	GSamples gData;
// Point lights
uniform int numPointLights;
uniform PointLight pointLight[32];

layout(std140) uniform viewPosBlock
{
    vec3 viewPos;
};

// Outputs
layout (location = 0) out vec4 FragColor;

void main()
{
    // Get the Geometic Pass data
    // vec3 FragPos = texture(gData.position, FragmentIn.TexCoords).xyz;
    // vec3 normal = texture(gData.normal, FragmentIn.TexCoords).xyz;
    vec3 albedo = texture(gData.albedo, FragmentIn.TexCoords).xyz;
    // float specular = texture(gData.albedo, FragmentIn.TexCoords).w;
    // vec3 viewDir = normalize(viewPos - FragPos);

    // Base lighting
    vec3 totalLight = albedo * 0.01;

    FragColor = vec4(totalLight, 1.0);  
}