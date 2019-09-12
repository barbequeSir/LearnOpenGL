#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * view * transform* vec4(position ,1.0f);
    TexCoord = texcoord;
}