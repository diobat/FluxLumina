#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;

// Uniforms
uniform mat4 model;			// Model matrix
uniform mat4 view;			// View matrix
uniform mat4 projection;	// Projection matrix

// Output
out vec3 objectColor;
out vec3 Normal;
out vec2 TexCoords;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);

	objectColor = aObjectColor;
	Normal = aNormal;
	TexCoords = aTexCoords;
	FragPos = vec3(model * vec4(aPosition, 1.0f));
}