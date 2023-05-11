//Fragment Shader 
#version 420

// In from the vertex shader
in  vec3 color;

// Out to the screen (or back buffer)
out vec4 finalOutputColour;	// to the screen

uniform vec3 colorOverrideRGB;

void main()
{
//	gl_FragColor = vec4(color, 1.0);
//	gl_FragColor = vec4(colorOverrideRGB, 1.0);
	finalOutputColour = vec4(colorOverrideRGB, 1.0);
}
