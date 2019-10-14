#version 330 core

in vec3 Normal;
in vec3 WorldPos;

uniform samplerCube Tex;
uniform vec3 CamPos;
out vec4 color;
void main()
{    
	vec3 viewDir = normalize(WorldPos - CamPos);
	vec3 normal = normalize(Normal);
	vec3 dir = reflect(viewDir,normal);

    color = texture(Tex,dir);
}