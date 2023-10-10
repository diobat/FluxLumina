#version 430 core

// Inputs
in vec2 TexCoords;

// Uniforms
uniform sampler2D screenTexture;

// Outputs
out vec4 FragColor;

void main()
{ 
    FragColor = texture(screenTexture, TexCoords);
}