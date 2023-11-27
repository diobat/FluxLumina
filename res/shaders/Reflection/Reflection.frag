#version 430

// Inputs
in vec3 Normal;
in vec3 Position;

// Uniforms

layout(std140) uniform viewPosBlock
{
	vec3 viewPos;
};

uniform samplerCube cubemap;

// Outputs
out vec4 fragColor;

void main()
{             
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    fragColor = vec4(texture(cubemap, R).rgb, 1.0);
}