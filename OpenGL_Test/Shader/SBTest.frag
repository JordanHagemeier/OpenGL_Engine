//#version 430 core
//out vec4 FragColor;
//
//in vec4 dummy;
//void main(){
//	
//
//	FragColor = dummy;
//}
//
#version 430 core
out vec4 FragColor;
  

in vec3 TexCoord;
in vec3 Dummy;

uniform samplerCube skybox;

uniform float value;

void main()
{
//	FragColor = Dummy.xyzx;
    FragColor = texture(skybox, TexCoord);
} 