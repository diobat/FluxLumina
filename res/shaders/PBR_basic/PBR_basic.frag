#version 430

struct Material{
sampler2D diffuse;
sampler2D specular;
float shininess;
sampler2D normal;
sampler2D height;
sampler2D roughness;    // Roughness in G channel, metallic in B channel
sampler2D ao;
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
in VertexData{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TS_FragPos;
	vec3 TS_ViewPos;
    vec3 TS_SphereCenter;
} fragIn;

// Uniforms
uniform Material material;
uniform int numPointLights;
uniform PointLight pointLight[10];
uniform mat4 model;

layout(std140) uniform viewPosBlock
{
	vec3 viewPos;
};

const float PI = 3.14159265359;

// Outputs
out vec4 fragColor;

// Functions
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
    // Normal and view vectors
    vec3 V = normalize(viewPos - fragIn.FragPos);
    vec2 TexUV = fragIn.TexCoords;

    vec3 N = texture(material.normal, TexUV).rgb;
    N = N * 2.0 - 1.0;   
    N = normalize(fragIn.TBN * N); 

    float roughness = texture(material.roughness, TexUV).g * 0.5;
    float metallic = texture(material.roughness, TexUV).b;
    vec3 albedo = pow(texture(material.diffuse, TexUV).rgb, vec3(2.2));
    float ao = texture(material.ao, TexUV).r;

    // Surface reflection at perpedicular incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numPointLights; i++)
    {
        // Light vector
        vec3 L = normalize(pointLight[i].position - fragIn.FragPos);

        // Halfway vector
        vec3 H = normalize(V + L);

        float distance = length(pointLight[i].position - fragIn.FragPos);
        // float attenuation = 1.0 / (pointLight[i].constant + pointLight[i].linear * distance + pointLight[i].quadratic * (distance * distance));
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLight[i].diffuse * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // Last element prevents division by zero
        vec3 specular = numerator / denominator;

        // Add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo + specular) * radiance * NdotL;
    }

    // Ambient occlusion
    vec3 ambient = vec3(0.003) * albedo;// * ao;
    vec3 color = ambient + Lo;
    color = Lo;

    // Gamma correction
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  
