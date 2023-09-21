#version 430

// Input Layout Locations
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;
layout(location = 3) in vec3 objectColor_in;


// Uniforms
uniform mat4 MVP;	// Model View Projection
uniform mat4 transform;	// Transform matrix

// Output
out vec3 objectColor;

void main()
{
	objectColor = objectColor_in;
	gl_Position = MVP * transform * vec4(position, 1.0f);
}