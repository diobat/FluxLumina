#version 430

// Input Layout Locations
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoords;

out VertexOutput
{
    vec2 TexCoords;
} VertexOut;


void main()
{
    VertexOut.TexCoords = aTexCoords;
    gl_Position = vec4(aPosition, 1.0);
}