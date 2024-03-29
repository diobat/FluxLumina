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
    fragColor = texture(material.diffuse, TexCoords);
}