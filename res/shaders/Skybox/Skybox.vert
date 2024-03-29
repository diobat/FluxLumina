#version 430
// Layouts
layout (location = 0) in vec3 aPos;

// Uniforms
uniform mat4 projection;
uniform mat4 view;

// Output
out vec3 TexCoords;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}