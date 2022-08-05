#version 430 core
out vec4 FragColor;
  

in vec2 TexCoord;
in vec3 Dummy;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float value;

void main()
{
//	FragColor = Dummy.xyzx;
//    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(TexCoord.x, TexCoord.y)), value);
	
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
} 