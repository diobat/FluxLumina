#version 430

struct Material
{
sampler2D diffuse;
};

// Inputs from the vertex shader
in vec3 objectColor;
in vec3 Normal;	
in vec2 TexCoords;
in vec3 FragPos;

//////////////////////////
// Uniforms
//////////////////////////
uniform Material material;

// Outputs
out vec4 fragColor;

void main()
{
    vec4 texColor = texture(material.diffuse, TexCoords);
    if(texColor.a < 0.1)
        discard;
    fragColor = texColor;
}