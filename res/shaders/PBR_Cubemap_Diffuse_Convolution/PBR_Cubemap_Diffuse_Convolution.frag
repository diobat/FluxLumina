#version 430

// Inputs
in vec3 fragPos;

// Uniforms
uniform samplerCube inputCubemap;

// Constants
const float PI = 3.14159265359;

// Outputs
out vec4 fragColor;

void main()
{
    vec3 N = normalize(fragPos);

    vec3 irradiance = vec3(0.0);

    // Calulate world space vector base
    vec3 up = vec3(0.0, 1.0, 0.0); 
    vec3 right = cross(up, N);
    up = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;

    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)    // Longitude all around the sphere
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)   // Latitude from bottom to top of the sphere
        {
            // Hop from spherical to cartesian coordinates
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // And then use the vector base to get the world space 
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(inputCubemap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    fragColor = vec4(irradiance, 1.0);
}