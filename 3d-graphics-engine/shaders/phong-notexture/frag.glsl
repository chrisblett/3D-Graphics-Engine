#version 430

#define MAX_POINT_LIGHTS 4
#define MAX_DIR_LIGHTS   4

#define MAX_LIGHT_MATRICES 4

struct Attenuation
{
	float quadratic;
	float linear;
	float constant;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// The index of the light's respective shadow map
	// in the shadow map array
	int shadowIndex;
};

struct DirectionalLight
{
	Light light;

	vec3 direction;
};

struct PointLight
{
	Light light;

	vec3 position;
	Attenuation attenuation;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

in vec3 positionWorld;
in vec3 normalWorld;
in vec4 shadowCoords[MAX_LIGHT_MATRICES];

out vec4 fragColour;

uniform int pointLightCount;
uniform int dirLightCount;

uniform sampler2DShadow shadowMaps[MAX_LIGHT_MATRICES];
uniform float shadowBiases[MAX_LIGHT_MATRICES];
uniform vec3 globalAmbientLight;
uniform vec3 eyePositionWorld;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLights[MAX_DIR_LIGHTS];
uniform Material material;

float CalculateShadowMapPCF(sampler2DShadow shadowMap, vec3 shadowCoordProj)
{
	// Perform custom PCF (percentage closer filtering)

	// The size of the 'window' to sample around the fragment
	const float WINDOW_SIZE = 5.0;

	const float TOTAL_SAMPLES = WINDOW_SIZE*WINDOW_SIZE;

	// How far to move to be centered around the current texel
	const float OFFSET = (WINDOW_SIZE - 1) / 2.0;

	float shadow = 0.0;

	for(float x = -OFFSET; x <= OFFSET; x += 1.0)
	{
	    for(float y = -OFFSET; y <= OFFSET; y += 1.0)
	    {
	          shadow += textureOffset(shadowMap, shadowCoordProj, ivec2(x, y)); 
	    }    
	}

	// Get the average of the values
	shadow /= TOTAL_SAMPLES;

	return shadow;
}

float CalculateShadow(sampler2DShadow shadowMap, vec4 shadowCoord,
vec3 N, vec3 L, int idx)
{
	vec3 shadowCoordProj = vec3(shadowCoord / shadowCoord.w);
	
	// Calculate the min and maximum bias
	const float MIN_BIAS = shadowBiases[idx];
	const float MAX_BIAS = shadowBiases[idx] * 10.0;

	// The bias should change depending on the angle to the light
	// instead of being constant.
	// This prevents shadow acne from appearing on sloped surfaces
	// despite not showing on straighter surfaces.
	float bias = max(MAX_BIAS * (1.0 - dot(N, L)), MIN_BIAS);  
	shadowCoordProj.z -= bias;

	//return texture(shadowMap, shadowCoordProj);
	return CalculateShadowMapPCF(shadowMap, shadowCoordProj);
}

vec3 CalculateLight(Light light, vec3 L, vec3 N, int idx)
{
	L = normalize(L);	

	// Diffuse
	float diffuseIntensity = max(dot(N, L), 0.0);

	// Specular

	// Eye dir vector
	vec3 E = normalize(eyePositionWorld - positionWorld);

	// The reflected light vector
	vec3 R = reflect(-L, N);
	float specFactor = pow(max(dot(R, E), 0.0), material.shininess);
	
	// Calculate the shadow amount
	float shadow = CalculateShadow(shadowMaps[idx], shadowCoords[idx], N, L, idx);

	vec3 ambient  = material.ambient * (globalAmbientLight + light.ambient);
	vec3 diffuse  = light.diffuse  * material.diffuse  * diffuseIntensity;
	vec3 specular = light.specular * material.specular * specFactor;

	return (diffuse + specular) * shadow + ambient;
}

vec3 CalculatePointLight(PointLight pointLight, vec3 N)
{
	vec3 L = pointLight.position - positionWorld;
	vec3 total = CalculateLight(pointLight.light, L, N, pointLight.light.shadowIndex);
	
	// Get the distance for attenuation calculations
	float distance = length(L);
	
	float attenuation = 1.0f / (pointLight.attenuation.quadratic * (distance * distance) + 
	pointLight.attenuation.linear * distance + pointLight.attenuation.constant);

	return attenuation * total;
}

vec3 CalculateDirectionalLight(DirectionalLight dirLight, vec3 N)
{
	vec3 L = -dirLight.direction;
	return CalculateLight(dirLight.light, L, N, dirLight.light.shadowIndex);
}

void main()
{	
	vec3 N = normalize(normalWorld);
	vec3 total;

	// Keeps track of the overall light count
	int shadowIdx = 0;  

	// Keeps track of the count of the current light type
	// This is reset when switching light types
	int lightIdx = 0;

	while(lightIdx < pointLightCount)
	{
		total += CalculatePointLight(pointLights[lightIdx], N);
		lightIdx++;
	}

	lightIdx = 0;

	while(lightIdx < dirLightCount)
	{
		total += CalculateDirectionalLight(directionalLights[lightIdx], N);
		lightIdx++;
	}

	fragColour = vec4(total, 1.0f);
}