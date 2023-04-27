#version 430

// Layouts
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;


// Uniforms
uniform mat4 MVP;
	


void main()
{

	
	gl_Position = MVP * vec4(position, 1.0f);
}