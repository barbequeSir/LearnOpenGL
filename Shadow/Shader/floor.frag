#version 330 core

in vec3 Normal;
in vec2 Texcoord;
in vec3 WorldPos;
in vec4 WorldPosLightSpace;

uniform vec3 LightPos;
uniform vec3 CameraPos;
uniform sampler2D shadowMap;

out vec4 color;

float ShadowCaculation(vec4 worldPosLightSpace)
{
	vec3 viewDir = normalize(CameraPos - WorldPos);
	vec3 lightDir = normalize(LightPos - WorldPos);
	vec3 normal = normalize(Normal);

	vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap,projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = 0.005;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	vec3 viewDir = normalize(CameraPos - WorldPos);
	vec3 lightDir = normalize(LightPos - WorldPos);
	vec3 normal = normalize(Normal);
	float diffuse = max(0,dot(lightDir,normal));	
	float shadow = ShadowCaculation(WorldPosLightSpace);
	color = vec4(0.2,0.7,0.3,1.0f) * diffuse * (1 - shadow + 0.1);
}