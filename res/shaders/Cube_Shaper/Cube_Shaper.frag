#version 430
// Inputs 
in vec3 FragPos;    

// Uniforms
uniform sampler2D textureMap;

// Outputs
out vec4 FragColor;

// Constants
const vec2 invAtan = vec2(0.1591, 0.3183); // 1 / (2 * PI)

// Functions
vec2 SampleSphericalMap(vec3 v);

void main()
{
    vec2 uv = SampleSphericalMap(normalize(FragPos));
    vec3 color = texture(textureMap, uv).rgb;

    FragColor = vec4(color, 1.0);
}

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
