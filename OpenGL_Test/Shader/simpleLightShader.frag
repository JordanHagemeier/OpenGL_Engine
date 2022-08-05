#version 430 core
out vec4 FragColor;

//in vec4 lightDirection;
//in vec3 normal;
//in vec4 originalColor;

//uniform vec4 lightColor;
//uniform float lightIntensity;
//

void main(){
	
//	float dLightSurface = dot(vec3(lightDirection.x, lightDirection.y, lightDirection.z), normal);
//	vec4 lightColWithFallOff = vec4(lightColor*dLightSurface);
//	vec4 lightedVersion = originalColor + vec4(lightColWithFallOff*lightIntensity);
//	FragColor = vec4(lightedVersion.xyz, 1.0);

	FragColor = vec4(0.6, 0.5, 0.2, 1);
}