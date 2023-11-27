#version 430 

// Layouts
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aObjectColor;

// Uniforms

layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};
uniform mat4 model;

// Outputs
out vec3 Normal;
out vec3 Position;

void main()
{
    Normal = mat3(transpose(inverse(model))) * aNormal;
    Position = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}  