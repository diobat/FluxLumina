#version 430

// Layouts
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;

// Uniforms
uniform mat4 MVP;	// Model View Projection
uniform mat4 transform;	// Transform matrix

void main()
{
	// gl_Position = MVP * transform * vec4(position, 1.0f);
	gl_Position = MVP * transform * vec4(position, 1.0f);
}