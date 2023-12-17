#version 430
// Inputs
layout (location = 0) in vec3 aPos;

// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};


// Outputs
out vec3 FragPos;

void main()
{
    FragPos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}