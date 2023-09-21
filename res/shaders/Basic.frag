#version 430

// Inputs from the vertex shader
in vec3 objectColor;

// Outputs
out vec4 fragColor;


void main()
{   
	fragColor = vec4(objectColor, 1.0);
}