#version 430

in vec2 textureCoords;

out vec4 fragColour;

uniform sampler2D shadowMap;

uniform float nearZ;
uniform float farZ;
uniform bool isOrtho;

// References: https://learnopengl.com/Advanced-OpenGL/Depth-testing
float LinearizeDepth(float depth)
{
	// Transform the depth value to normalized device coordinates (NDC).
    float z = depth * 2.0 - 1.0; 
    return (2.0 * nearZ * farZ) / (farZ + nearZ - z * (farZ - nearZ));
}

void main()
{
	float depth = texture(shadowMap, textureCoords).r;

	// If the projection is perspective, the depth must be 'linearized'
	// since the depth buffer is non-linear
	if(!isOrtho)
	{
		depth = LinearizeDepth(depth) / farZ;
	}

	fragColour = vec4(vec3(depth), 1.0);
}