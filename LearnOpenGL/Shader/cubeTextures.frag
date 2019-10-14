#version 330 core

in vec3 TexCoord;

uniform samplerCube Tex;

out vec4 color;
void main()
{    
 	color = texture(Tex,TexCoord);
}