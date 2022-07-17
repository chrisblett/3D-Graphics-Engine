#version 430

layout (location=0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;

out vec3 texCoord;

void main()
{
	// Set the vector to sample the cubemap as the position vector of the vertex,
	// so it can be interpolated
	texCoord = position;

	// 'Turn off' translation since the skybox should only rotate
	gl_Position = projection * mat4(mat3(view)) * vec4(position.xyz, 1.0);
}