#version 430

// Inputs from the vertex shader
in vec3 objectColor;
in vec2 TexCoords;

// Uniforms
uniform int sampleFromTexture;
uniform sampler2D texture0;

// Outputs
out vec4 fragColor;


void main()
{   
	if(sampleFromTexture == 0)
		fragColor = vec4(objectColor, 1.0);
	else
		fragColor = texture(texture0, TexCoords);
	//
}