// Vertex shader
#version 420

uniform mat4 MVP;

// Coming in from the vertex layout
in vec3 vCol;		
in vec3 vPos;

// Going out to the Fragment shader
out vec3 color;

void main()
{
	vec3 position = vPos;

//	position.x += 2.0f;
//	position.x *= 0.2f;
//	position.y *= 0.2f;
//	position.z *= 0.2f;
	
    gl_Position = MVP * vec4(position, 1.0);
    color = vCol;
	
}// off to the fragment shader