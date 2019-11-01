#version 330 core

out vec4 color;

in vec2 TexCoord;
in vec3 Normal;
in vec3 worldPos;

vec3 cameraPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

void main()
{    
	color = vec4(worldPos,1.0);
}