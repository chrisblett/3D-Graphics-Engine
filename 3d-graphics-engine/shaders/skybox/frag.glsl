#version 430

uniform samplerCube sampler;

in vec3 texCoord;

out vec4 fragColour;

void main()
{
	fragColour = texture(sampler, texCoord);
}