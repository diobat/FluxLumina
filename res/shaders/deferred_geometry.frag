#version 430

// Structs
struct Material{
sampler2D diffuse;
sampler2D specular;
float shininess;
sampler2D normal;
sampler2D height;
};

////////// Inputs
in VertexOutput{
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPos;
} FragmentIn;

////////// Uniforms
uniform Material material;

////////// Outputs
layout (location = 0) out vec4 oPosition;
layout (location = 1) out vec4 oNormal;
layout (location = 2) out vec4 oAlbedoSpec;

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    oPosition = vec4(FragmentIn.FragPos.rgb, 1.0);
    // also store the per-fragment normals into the gbuffer
    oNormal = vec4(normalize(FragmentIn.Normal), 1.0);
    // and the diffuse per-fragment color
    oAlbedoSpec.rgb = texture(material.diffuse, FragmentIn.TexCoords).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    oAlbedoSpec.a = texture(material.specular, FragmentIn.TexCoords).r;
}  