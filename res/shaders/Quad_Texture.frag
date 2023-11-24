#version 430 

struct Material
{
    sampler2D color;
};

// Inputs
in vec2 TexCoords;

// Uniforms
uniform Material tex;
layout(std140) uniform viewPortBlock
{
    vec2 viewPortSize;
};


// Outputs
layout (location = 0) out vec4 FragColor;

void main()
{ 

    vec2 screenSpace;
    screenSpace.x = gl_FragCoord.x / viewPortSize.x;
    screenSpace.y = gl_FragCoord.y / viewPortSize.y;


    FragColor  = vec4(texture(tex.color, screenSpace).rgb, 1.0);
}