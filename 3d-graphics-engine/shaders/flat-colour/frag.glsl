#version 430

uniform vec3 diffuseColour;

out vec4 colour;

void main()
{
	colour = vec4(diffuseColour, 1.0f);
}