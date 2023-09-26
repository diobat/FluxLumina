#version 430

// Inputs from the vertex shader
in vec3 objectColor;
in vec3 Normal;	
in vec2 TexCoords;
in vec3 FragPos;

// Uniforms
uniform int sampleFromTexture;
uniform sampler2D texture0;

uniform vec3 ambColor;
uniform float ambIntensity;

uniform vec3 diffDirection;
uniform vec3 diffColor;
uniform float diffIntensity;

// Outputs
out vec4 fragColor;


void main()
{   
	// Sample from texture or use object color
	if(sampleFromTexture == 0)
		fragColor = vec4(objectColor, 1.0);
	else
		fragColor = texture(texture0, TexCoords);


	// Ambient light
	vec3 ambient = ambIntensity * ambColor;

	// Diffuse light
	vec3 diffLightDir = normalize(diffDirection);
	// The impact of diff light on the vertex is the dot product of the light direction and the normal
	float diff = max(dot(Normal, diffLightDir), 0.0);
	vec3 diffuse = diff * diffColor * diffIntensity;

	vec4 resultColor = vec4(ambient,1.0) + vec4(diffuse, 1.0) + fragColor ;
	//vec4 resultColor = vec4(ambient+,1.0) + vec4(diffuse,1.0) + fragColor;

	fragColor = resultColor;
}