#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthTexture;
uniform sampler2D colorTexture;

void main()
{    
//		FragColor = 1-(smoothstep(0.96, 1.01, texture(depthTexture, TexCoords)).xxxx);
		FragColor = texture(colorTexture, TexCoords) * vec4((1-(smoothstep(0.96, 1.07, texture(depthTexture, TexCoords)).x)),(1-(smoothstep(0.96, 1.01, texture(depthTexture, TexCoords)).x)),1.0, 1.0) ;
//		FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}