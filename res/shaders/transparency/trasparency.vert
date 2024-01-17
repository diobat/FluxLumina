#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;

// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};
uniform mat4 model;

// Outputs
out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}