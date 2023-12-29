#version 430
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 shadowModel;

void main()
{
    gl_Position = lightSpaceMatrix * shadowModel * vec4(aPos, 1.0);
}  