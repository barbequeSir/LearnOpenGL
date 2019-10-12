#version 330 core

out vec4 color;
void main()
{    
    //color = vec4(0.5,0.5,0.5,1.0);    
    float near= 0.1;
    float far = 100.0;
    float z = gl_FragCoord.z * 2.0 - 1.0;
    float linerDepth = 2.0*near /(near + far - z*(far - near));
    color = vec4(vec3(linerDepth),1.0);
}