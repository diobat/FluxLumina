#version 430
// Inputs
layout (location = 0) in vec3 aPos;

// Uniforms
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

// Outputs
out vec3 fragPos;

void main()
{
    fragPos = aPos;
    gl_Position = projectionMatrix * viewMatrix * vec4(aPos, 1.0);
}