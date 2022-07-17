#version 430

#define MAX_LIGHT_MATRICES 4

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 positionWorld;
out vec3 normalWorld;
out vec4 shadowCoords[MAX_LIGHT_MATRICES];

uniform mat4 modelViewProjMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 lightMatrices[MAX_LIGHT_MATRICES];

void main()
{
	gl_Position = modelViewProjMatrix * vec4(position, 1.0);

	// Transform the vertex position to world space for lighting calculations
	positionWorld = vec3(modelToWorldMatrix * vec4(position, 1.0));

	// Make the w component 0 to ignore translation
	normalWorld = vec3(modelToWorldMatrix * vec4(normal, 0.0));

	textureCoord = texCoord;

	for(int i = 0; i < MAX_LIGHT_MATRICES; ++i)
	{
		shadowCoords[i] = lightMatrices[i] * vec4(position, 1.0);
	}
}