#version 330 core

in vec3 Normal;
in vec2 Texcoord;
in vec3 WorldPos;

uniform vec3 CameraPos;
uniform vec3 LightPos;

out vec4 color;
void main()
{
	vec3 viewDir = normalize(CameraPos - WorldPos);
	vec3 lightDir = normalize(LightPos - WorldPos);
	vec3 normal = normalize(Normal);
	float diffuse = max(0,dot(lightDir,normal));

	color = vec4(0.7,0.2,0.2,1.0) * diffuse;
}