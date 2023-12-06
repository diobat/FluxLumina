#version 430
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aObjectColor;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in mat4 instanceMatrix;


// Uniforms
layout(std140) uniform mvp_camera 
{
	mat4 view;
	mat4 projection;
};
layout(std140) uniform viewPosBlock
{
	vec3 viewPos;
};
uniform mat4 model;
uniform int sampleFromNormal;
uniform int sampleFromHeight;

// Outputs
out VertexData{
    vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
    mat3 TBN;
    vec3 TS_FragPos;
	vec3 TS_ViewPos;
    vec3 TS_SphereCenter;
} VertexOut;

void main()
{
    VertexOut.Normal = mat3(transpose(inverse(model))) * aNormal;
    VertexOut.TexCoords = aTexCoords;
    VertexOut.FragPos = vec3(model * vec4(aPosition, 1.0));

    if(sampleFromNormal	== 1 || sampleFromHeight == 1)
	{
		vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
		vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
		vec3 B = cross(N, T);	
		VertexOut.TBN = mat3(T, B, N);
		VertexOut.TS_FragPos = inverse(VertexOut.TBN) * VertexOut.FragPos;
		VertexOut.TS_ViewPos = inverse(VertexOut.TBN) * viewPos;
        VertexOut.TS_SphereCenter = inverse(VertexOut.TBN) * model[3].xyz;
	}

    gl_Position = projection * view * vec4(VertexOut.FragPos, 1.0);
}