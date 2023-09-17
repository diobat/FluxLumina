#version 430

layout(location = 0) out vec4 fragColor;


void main()
{   
	float ambientWeight = 1.0;
	vec3 ambientLight = ambientWeight * vec3(0.7, 0.7, 0.7);

	vec3 objectColor = vec3(0.1, 0.5, 0.3);
	vec3 result = objectColor * ambientLight;

	fragColor = vec4(result, 1.0);
}