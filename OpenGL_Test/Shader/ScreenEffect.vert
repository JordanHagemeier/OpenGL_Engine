#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uvs;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = uvs;
    gl_Position = vec4(aPos, 1.0);
}  