#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;// the position variable has attribute position 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec4 fragColor;

out vec4 lightDirection;
out float lightDistance;

uniform vec4 ourColor;
uniform float offset;



uniform vec3 lightPos;


//lets try several lights

uniform vec3 lightPositions[2];
out vec4 lightDirections[2];
out float lightDistances[2];
//out float lightDistances[2];


//for texturing learning purposes

out vec2 TexCoord;

void setLightDirectionAndDistance();
void main()
{

	gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragColor = ourColor;
	normal = vec3(normalize(model * vec4(aNormal, 1.0)));

	lightDirection =  normalize(vec4(lightPos.xyz - aPos, 1.0));
	lightDistance = distance(  vec4(lightPos, 1.0),model * vec4(aPos, 1.0)) / 5;
	
	setLightDirectionAndDistance();

	TexCoord = aTexCoord;

}

void setLightDirectionAndDistance(){
	for(int i = 0; i < 2; i++){
	lightDirections[i] = normalize(vec4(lightPositions[i].xyz - aPos, 1.0));
	lightDistances[i] = distance(  vec4(lightPositions[i], 1.0),model * vec4(aPos, 1.0)) / 5;
	}
}