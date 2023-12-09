#version 430

// Inputs
in vec3 TexCoords;

// Uniforms
uniform samplerCube skyboxCube;

// Outputs
out vec4 FragColor;


void main()
{    
    vec3 color = texture(skyboxCube, TexCoords).rgb;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
  
    FragColor = vec4(color,1.0);
}