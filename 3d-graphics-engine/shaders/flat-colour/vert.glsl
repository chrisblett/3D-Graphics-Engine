#version 430

layout (location=0) in vec3 position;

uniform mat4 modelViewProjMatrix;

void main()
{
	gl_Position = modelViewProjMatrix * vec4(position.xyz, 1.0f);
}