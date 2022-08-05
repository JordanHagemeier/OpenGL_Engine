#version 430 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;
//layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

//uniform vec3 lightPos;

//out vec4 lightDirection;
//out vec3 normal;

void main(){
//	normal = aPos;
//	lightDirection = vec4(lightPos.xyz - aPos, 1.0);
	gl_Position = perspective * view * model * vec4(aPos, 1.0);

}