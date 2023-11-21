#version 430

// Uniforms
uniform vec4 outputColor;

// Outputs
out vec4 fragColor;

void main()
{
	fragColor = outputColor;
	// fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
