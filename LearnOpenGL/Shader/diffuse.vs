#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 WorldPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

vec3 worldPos;

void main()
{
	gl_Position = projection * view * transform * vec4(position,1.0f);
	WorldPos = vec3(transform * vec4(position,1.0f));
	Normal =  mat3(transpose(inverse(transform))) * normal;
}