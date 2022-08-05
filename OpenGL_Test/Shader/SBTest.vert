//#version 430 core
//layout (location = 0) in vec3 aPos; 
//
//
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 perspective;
//
//out vec4 dummy;
//
//void main(){
//
//	dummy = vec4(aPos, 1.0);
//	gl_Position = perspective * view * model * vec4(aPos, 1.0);
//
//}

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;



out vec3 TexCoord;
out vec3 Dummy;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 dummy;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * vec4(aPos, 1.0);
    
    TexCoord	=  aPos;
	Dummy		=  vec3(1.0, 0.0, 0.0);
}