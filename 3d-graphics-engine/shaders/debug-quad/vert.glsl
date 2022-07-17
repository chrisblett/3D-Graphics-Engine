#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec2 texCoord;

uniform mat4 scale;
uniform mat4 translation;

out vec2 textureCoords;

void main()
{
	gl_Position = translation * scale * vec4(position, 1.0);
	textureCoords = texCoord;
}