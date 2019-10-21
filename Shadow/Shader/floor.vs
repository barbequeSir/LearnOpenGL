#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec2 Texcoord;
out vec3 WorldPos;
void main()
{
	gl_Position = proj * view * model * vec4(position,1.0);
	Texcoord = texcoord;
	Normal = mat3(transpose(inverse(model))) * normal;
	WorldPos = vec3(model * vec4(position,1.0));
}