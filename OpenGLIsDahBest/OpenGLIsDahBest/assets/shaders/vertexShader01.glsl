// Vertex shader
#version 420

uniform mat4x4 matModel;			// M
uniform mat4x4 matView;				// V
uniform mat4x4 matProjection;		// P
//uniform mat4 MVP;

// Coming in from the vertex layout
in vec3 vCol;		
in vec3 vPos;

// Going out to the Fragment shader
out vec3 color;

void main()
{
	vec3 position = vPos;
	
//	vec4 = mat4x4 * vec4;
// From line 439 of the C++ main file
// mvp = p * v * m;
	
	mat4 matMVP = matProjection * matView * matModel;
    gl_Position = matMVP * vec4(position, 1.0);
    color = vCol;
	
}// off to the fragment shader