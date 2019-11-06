#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texcoord;

layout(location = 6) in mat4 matModel;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
out vec2 TexCoords;

void main()
{	
	gl_Position =  projection * view * matModel * vec4(position,1.0);
	TexCoords = texcoord;
}