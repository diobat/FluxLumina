#version 430

// Inputs
in vec3 TexCoords;

// Uniforms
uniform samplerCube skybox;

// Outputs
out vec4 FragColor;


void main()
{    
    FragColor = texture(skybox, TexCoords);
}