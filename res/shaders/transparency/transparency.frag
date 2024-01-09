#version 430

struct Material
{
sampler2D diffuse;
};

// Inputs from the vertex shader
in vec2 TexCoords;

//////////////////////////
// Uniforms
//////////////////////////
uniform Material material;

// Outputs
out vec4 fragColor;

void main()
{
    fragColor = texture(material.diffuse, TexCoords);
}