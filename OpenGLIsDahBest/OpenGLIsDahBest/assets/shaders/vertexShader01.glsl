#version 110

uniform mat4 MVP;

attribute vec3 vCol;		
attribute vec3 vPos;

varying vec3 color;

void main()
{
	vec3 position = vPos;
	
//	position.x += 2.0;
//	position.x *= 0.2;
//	position.y *= 0.2;
//	position.z *= 0.2;
	
	
    gl_Position = MVP * vec4(position, 1.0);
    color = vCol;
}