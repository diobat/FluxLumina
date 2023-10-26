#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;
layout(location = 4) in mat4 instanceMatrix;
//layout(location = 5) in mat4 instanceMatrix;		// Taken due to mat4x4
//layout(location = 6) in mat4 instanceMatrix;		// Taken due to mat4x4
//layout(location = 7) in mat4 instanceMatrix;		// Taken due to mat4x4

// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};
uniform mat4 model;

// Output
out vec3 objectColor;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(aPosition, 1.0f);

	objectColor = aObjectColor;
	Normal = aNormal;
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPosition, 1.0f));
}