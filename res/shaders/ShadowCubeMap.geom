#version 430

// Input Layout Locations
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

// Uniforms
uniform mat4 lightSpaceMatrix[6];

// Output
out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = lightSpaceMatrix[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  