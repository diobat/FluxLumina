#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in mat4 instanceMatrix;
//layout(location = 6) in mat4 instanceMatrix;		// Taken due to mat4x4
//layout(location = 7) in mat4 instanceMatrix;		// Taken due to mat4x4
//layout(location = 8) in mat4 instanceMatrix;		// Taken due to mat4x4

// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};

// Output
out VertexOutput{
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPos;
    vec3 objectColor;   
} VertexOut;

void main()
{
    vec4 worldPos = instanceMatrix * vec4(aPosition, 1.0);
    VertexOut.FragPos = worldPos.xyz; 
    VertexOut.TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(instanceMatrix)));
    VertexOut.Normal = normalMatrix * aNormal;

    VertexOut.objectColor = aObjectColor;

    gl_Position = projection * view * worldPos;
}