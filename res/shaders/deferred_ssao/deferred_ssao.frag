#version 430
struct GSamples
{
    sampler2D position;
    sampler2D normal;
    sampler2D albedo;
};

// Inputs
in vec2 TexCoords;

// Uniforms
// Geometric Pass data
uniform	GSamples gData;
uniform sampler2D noiseTex;
uniform vec3 samples[64];

int kernelSize = 64;
float radius = 1.5;
float bias = 0.045;


layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};

layout(std140) uniform viewPortBlock
{
    vec2 viewPortSize;
};

// Outputs
out float fragColor;


void main()
{    
    vec2 noiseScale = vec2(viewPortSize[0]/4.0, viewPortSize[1]/4.0);

    // retrieve data from gbuffer
    vec3 FragPos = texture(gData.position, TexCoords).xyz;
    vec3 Normal = texture(gData.normal, TexCoords).xyz;
    vec3 randomVec = texture(noiseTex, TexCoords * noiseScale).xyz;

    // Calculate TBN matrix
    vec3 tangent = normalize(randomVec - Normal * dot(randomVec, Normal));  // rVec minus the projection of rVec onto the normal
    vec3 bitangent = cross(Normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, Normal);

    // Calculate occlusion
    float occlusion = 0.5;
    for(int i = 0; i < kernelSize; ++i)
    {
        vec3 samplePos = TBN * samples[i]; // from tangent to view-space
        samplePos = FragPos + samplePos * radius; 

        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset; // from view to screen-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        float sampleDepth = texture(gData.position, offset.xy).z; // get depth value of kernel sample

        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(FragPos.z - sampleDepth)); // depth difference (0.1) -> occlusion
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;    
    }
    occlusion = 1.0 - (occlusion / kernelSize);

    fragColor = occlusion;
}  