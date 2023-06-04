// Vertex shader
#version 420

uniform mat4x4 matModel;			// M : where's my model at?
uniform mat4x4 matView;				// V : where's the camera at?
uniform mat4x4 matProjection;		// P : what's the screen doing?
// For lighting - only impacts the normal 
uniform mat4x4 matModel_InvTrans;	// Inverse transpose of the model matrix
									// Used only for the normal, and strips out
									// and scaling and translation, leaves only rotation

//uniform mat4 MVP;

// Coming in from the vertex layout
in vec3 vCol;		
in vec3 vPos;
in vec3 vNormal;
in vec2 vUV;		// Texture coordinates

// Going out to the Fragment shader
out vec3 color;
out vec3 fNormal;		// 'f' for fun or fromage or fragment
out vec4 fVertexPosWorld;		// Where this vertex is in 'world space'
out vec2 fUV;

void main()
{
	vec3 position = vPos;
	
//	vec4 = mat4x4 * vec4;
// From line 439 of the C++ main file
// mvp = p * v * m;
	
	mat4 matMVP = matProjection * matView * matModel;
	
	// The location of the vertex in 'world' space
	fVertexPosWorld = matModel * vec4(position, 1.0);
	
	// Location of vertex in 'screen space'
    gl_Position = matMVP * vec4(position, 1.0);
	
    color = vCol;
	// Note that the mat4 x vec4 is needed,
	// mat4 * vec4 --> vec4
	//	then the results have to be changed to a vec3
	fNormal = ( matModel_InvTrans * vec4(normalize(vNormal), 1.0) ).xyz;
	
	// Pass the UVs along to the fragment shader
	fUV = vUV;
}// off to the fragment shader