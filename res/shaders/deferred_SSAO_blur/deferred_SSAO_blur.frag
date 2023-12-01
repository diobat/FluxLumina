#version 430

// Inputs
in vec2 TexCoords;

// Uniforms
uniform sampler2D screenTexture;

// Outputs
layout (location = 0) out float FragColor;

void main()
{
    vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
    
    float result = 0.0;

    for(int x = -2; x < 2; ++x)
    {
        for(int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(x, y) * texelSize;
            result += texture(screenTexture, TexCoords + offset).r;
        }
    }

    FragColor = result / (4.0 * 4.0);
}