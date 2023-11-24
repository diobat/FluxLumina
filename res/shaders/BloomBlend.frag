#version 430

struct Material{
sampler2D diffuse;
sampler2D specular;
};

// Input
in vec2 TexCoords;

// Output
out vec4 FragColor;

// Uniforms
uniform Material material;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(material.diffuse, TexCoords).rgb;      
    vec3 bloomColor = texture(material.specular, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * 2.0);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}  