#version 330 core

out vec4 color;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D ourTexture;
void main()
{   
    color = texture(ourTexture,TexCoord);
}