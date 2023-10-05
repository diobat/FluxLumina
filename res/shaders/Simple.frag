#version 430

// Inputs from the vertex shader
in vec3 objectColor;
in vec3 Normal;	
in vec2 TexCoords;
in vec3 FragPos;


// Outputs
out vec4 fragColor;

void main()
{
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
