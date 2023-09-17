#version 430

// Input Layout Locations
//layout(location = 3) in vec3 objectColor;


// Output Layout Locations
in vec3 objectColor;
out vec4 fragColor;


void main()
{   
	fragColor = vec4(objectColor, 1.0);
}