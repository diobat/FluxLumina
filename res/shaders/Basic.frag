#version 430

// Inputs from the vertex shader
in vec3 objectColor;
in vec2 TexCoords;

// Uniforms
uniform sampler2D texture0;

// Outputs
out vec4 fragColor;


void main()
{   
	fragColor = texture(texture0, TexCoords);
	//fragColor = vec4(objectColor, 1.0);
}