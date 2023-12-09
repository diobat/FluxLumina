#version 430
// Inputs 
in vec3 FragPos;    

// Uniforms
uniform sampler2D textureMap;

// Outputs
out vec3 FragColor;

// Constants
const vec2 invAtan = vec2(0.1591, 0.3183); // 1 / (2 * PI) and 1 / (PI)

// Functions
vec2 SampleSphericalMap(vec3 v);

void main()
{
    vec2 uv = SampleSphericalMap(normalize(FragPos.xyz));
    vec3 color = texture(textureMap, uv).rgb;

    FragColor = color;
}

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
