#version 430
// Inputs
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

// Outputs
out vec2 TexCoords;

void main()
{
    TexCoords = aTex;
	gl_Position = vec4(aPos, 1.0);
}