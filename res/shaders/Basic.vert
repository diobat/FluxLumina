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
uniform int numSpotLights;
uniform mat4 lightSpaceMatrix[20];

// Output
out VertexOutput{
	vec3 objectColor;
	vec3 Normal;
	vec2 TexCoords;
	vec3 FragPos;
} VertexOut;

out LightSpaceVertexOutput{
	vec4 Spotlight[20];
} LightSpaceVertexOut;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(aPosition, 1.0f);

	VertexOut.objectColor = aObjectColor;
	VertexOut.Normal = aNormal;
	VertexOut.TexCoords = aTexCoords;
	VertexOut.FragPos = vec3(instanceMatrix * vec4(aPosition, 1.0f));

	for(int i = 0; i < numSpotLights; i++)
	{
		// Vertex position in worldspace of light i = lightSpaceMatrix * vertex position in worldspace
		LightSpaceVertexOut.Spotlight[i] = lightSpaceMatrix[i] * vec4(VertexOut.FragPos, 1.0f);
	}
}