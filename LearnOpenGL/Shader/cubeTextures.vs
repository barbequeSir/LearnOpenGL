#version 330 core

layout (location = 0) in vec3 position;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoord;
void main()
{
	gl_Position = projection*view*  vec4(position,1.0f);

	TexCoord = position;
}