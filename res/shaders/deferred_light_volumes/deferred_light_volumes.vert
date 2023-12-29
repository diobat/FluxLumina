#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;


// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};
uniform mat4 model;

out VertexOutput
{
    vec2 TexCoords;
} VertexOut;


void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}