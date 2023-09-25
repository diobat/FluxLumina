#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;


// Uniforms
uniform mat4 MVP;	// Model View Projection
uniform mat4 transform;	// Transform matrix

// Output
out vec3 objectColor;
out vec2 TexCoords;

void main()
{
	objectColor = aObjectColor;
	TexCoords = aTexCoords;
	gl_Position = MVP * transform * vec4(aPosition, 1.0f);
}