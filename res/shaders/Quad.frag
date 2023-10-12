#version 430 core


struct Material
{
sampler2D diffuse;
sampler2D specular;
float shininess;
};


// Inputs
in vec2 TexCoords;

// Uniforms
uniform Material material;

// Outputs
out vec4 FragColor;

void main()
{ 
    FragColor = texture(material.diffuse, TexCoords);
}