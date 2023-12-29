#version 430

struct Material
{
sampler2D diffuse;
sampler2D specular;
float shininess;
};


// Inputs
in vec2 TexCoords;

// Uniforms
uniform Material material;

// Outputs
out vec4 FragColor;

void main()
{ 
    const float gamma = 2.2;
    vec3 hdrColor = texture(material.diffuse, TexCoords).rgb;
  
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}