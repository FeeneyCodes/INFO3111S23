//Fragment Shader 
#version 420

// In from the vertex shader
in vec3 color;
in vec3 fNormal;	
in vec4 fVertexPosWorld;
in vec2 fUV;

// Out to the screen (or back buffer)
// RGB, A  (a for "alpha")
out vec4 finalOutputColour;	// to the screen

uniform vec3 diffuseColor_OverrideRGB;		// was: ColorOverride
uniform vec4 specularColourRGB_Power;// = vec4(1.0f, 1.0f, 1.0f, 1.0f);
				// RGB is colour of the highlight
				// Power is the specular power or shininess

uniform float alphaTransparency;

uniform bool bDontLight;

// Location of the camera (or eye)
uniform vec3 eyeLocation;


struct sLight
{
	vec4 position;	// xyz, ignoring w (4th parameter)	
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights, ignoring w (4th parameter)	
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};
// Size of array MUST be known at compile time
const int NUMBEROFLIGHTS = 10;
uniform sLight theLights[NUMBEROFLIGHTS];

const int POINT_LIGHT = 0;					// Default.
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

uniform sampler2D texture00;		// Tay's Tay's face
uniform sampler2D texture01;		// John Wick
uniform sampler2D texture02;		// John Wick
uniform sampler2D texture03;		// John Wick

//uniform float texture00mix;// = 1.0f;
//uniform float texture01mix;// = 0.0f;
//uniform float texture02mix;// = 0.0f;
//uniform float texture03mix;// = 0.0f;
// All registers are vec4
uniform vec4 textureMix_00_to_03;	//x is #0, y is #1, etc.

//vec3 myOneLightPos = vec3(-5.0f, 15.0f, 0.0f);

// Just like C and C++, the compiler compiled from top to bottom
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

void main()
{
//	gl_FragColor = vec4(color, 1.0);
//	gl_FragColor = vec4(colorOverrideRGB, 1.0);
//	finalOutputColour = vec4(colorOverrideRGB, 1.0);
	
	
//	finalOutputColour = calcualteLightContrib(...._)
//
//	finalOutputColour = vec4( fNormal.xyz, 1.0f );
//	finalOutputColour = vec4( fVertexPosWorld.xyz, 1.0f );
//	
	// Calculate how much light has hit this vertex. 
//	vec3 lightVector = theLights[0].position.xyz - fVertexPosWorld.xyz;
//	float distanceToLight = length(lightVector);
	// 1 -> 1
	// 10 -> 0.1
	// 100 -> 0.01
//	
//	float brightness = (1.0f/(distanceToLight));
//	
//	brightness *= y_attenuation;
//	
//	brightness *= 10.0f;
//	
//	finalOutputColour.rgba = vec4((colorOverrideRGB * brightness), 1.0);

	if ( bDontLight )
	{
		finalOutputColour.rgb = diffuseColor_OverrideRGB.rgb;
//		finalOutputColour.w = 1.0f;
		finalOutputColour.w = alphaTransparency;
		return;
	}
	
	// Texture sampling cost "noting"
	vec3 textSample00_RGB = texture( texture00, fUV.xy ).rgb;	 
	vec3 textSample01_RGB = texture( texture01, fUV.xy ).rgb;	 
	vec3 textSample02_RGB = texture( texture02, fUV.xy ).rgb;	 
	vec3 textSample03_RGB = texture( texture03, fUV.xy ).rgb;	 

	// Mix these
//	float texture00mix = 1.0f;
//	float texture01mix = 0.0f;
//	float texture02mix = 0.0f;
//	float texture03mix = 0.0f;
	
	
//	vec3 finalText_RGB =   (textSample00_RGB * texture00mix) 		
//	                     + (textSample01_RGB * texture01mix) 
//	                     + (textSample02_RGB * texture02mix) 
//	                     + (textSample03_RGB * texture03mix);
						 
	vec3 finalText_RGB =   (textSample00_RGB * textureMix_00_to_03.x) 		
	                     + (textSample01_RGB * textureMix_00_to_03.y) 
	                     + (textSample02_RGB * textureMix_00_to_03.z) 
	                     + (textSample03_RGB * textureMix_00_to_03.w);
	
//	finalOutputColour = calcualteLightContrib(diffuseColor_OverrideRGB.rgb,
	finalOutputColour = calcualteLightContrib(finalText_RGB.rgb,
	                                          fNormal.xyz, 
											  fVertexPosWorld.xyz, 
											  specularColourRGB_Power );
										
//	finalOutputColour.rgb *= 0.00001f;
//	finalOutputColour.rgb += fNormal.rgb;
										
	finalOutputColour.w = alphaTransparency;
}



// Feeney gave you this (it's inspired by the basic shader in Mike Bailey's Graphic Shaders book)
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += ( vertexMaterialColour.rgb * 
			                           theLights[index].diffuse.rgb * 
									   lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;	
		float distanceToLight = length(vLightToVertex);		
		vec3 lightVector = normalize(vLightToVertex);		
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
					   
		// Attenuation
		// 1->2  --> dimmer by 2x
		// 1->2  --> dimmer by 4x
		// 1->10 --> dimmer by 10x
		// 1->10 --> dimmer by 100x
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight * distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb * lightSpecularContrib.rgb );
	}//for(intindex=0...
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}