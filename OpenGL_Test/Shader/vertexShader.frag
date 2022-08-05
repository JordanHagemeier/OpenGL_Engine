#version 430 core
out vec4 FragColor;
  
in vec4 fragColor; // the input variable from the vertex shader (same name and same type)  
in vec3 normal;

in vec4 lightDirection;
in float lightDistance;

uniform vec4 lightColor;
uniform float lightIntensity;
uniform float radius;

//several lights

in vec4 lightDirections[2]; //X
in float lightDistances[2]; //X


uniform vec4 lightColors[2];
uniform float lightIntensities[2];
uniform float radiuses[2];

vec4 calculateFragmentColor(int currentFragment);


//for texturing learning purposes
in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2; //I'm using the same names for the uniforms as the ones in the texturing shader

void main()
{
	float falloff = clamp(1- (lightDistance/radius), 0, 1);
	float dLightSurface =clamp(dot(vec3(lightDirection.x, lightDirection.y, lightDirection.z), normal), 0, 1);
	vec4 oneLight = vec4((dLightSurface * fragColor) * falloff * lightIntensity * lightColor);
//	FragColor = vec4((dLightSurface * fragColor) * falloff * lightIntensity * lightColor);

	//calculating em several lights
	float falloffs[2];
	float LightSurfacesDotProducts[2];
	vec4 tempFragColors[2];

	for(int i = 0; i < 2; i++){
		tempFragColors[i] = calculateFragmentColor(i);
	}

	FragColor = tempFragColors[0] + tempFragColors[1] + oneLight;
//	FragColor = texture(texture1, TexCoord);

//	FragColor = vec4(normal, 1.0);

} 

vec4 calculateFragmentColor(int currentFragment){
	float falloff;
	float LightSurfacesDotProduct;
	vec4 tempFragColor;

	falloff = 1- (lightDistances[currentFragment]/radiuses[currentFragment]);
	LightSurfacesDotProduct = clamp(dot(vec3(lightDirections[currentFragment].x, lightDirections[currentFragment].y, lightDirections[currentFragment].z), normal), 0, 1);
	tempFragColor = vec4((LightSurfacesDotProduct* fragColor) * falloff * lightIntensities[currentFragment] * lightColors[currentFragment]);
	return tempFragColor;
}