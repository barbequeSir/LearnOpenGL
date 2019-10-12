#version 330 core

in vec2 TexCoord;
uniform sampler2D texture1;

out vec4 color;
const float offset = 1.0 / 300;  
void main()
{    
    vec4 c = texture(texture1,TexCoord);       
    //color = vec4(vec3(1.0 - c),1.0);

    //float f = (c.x + c.y + c.z)/3.0;
    //color = vec4(f,f,f,1.0);
    
    

    /*
     vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0f,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0f),    // center-left
        vec2(0.0f,    0.0f),    // center-center
        vec2(offset,  0.0f),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right
    );
    //锐化
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    //模糊
    float kernel[9] = float[](
	    1.0 / 16, 2.0 / 16, 1.0 / 16,
	    2.0 / 16, 4.0 / 16, 2.0 / 16,
	    1.0 / 16, 2.0 / 16, 1.0 / 16  
	);
	//边缘检测
	float kernel[9] = float[](
	    1.0 , 1.0 , 1.0 ,
	    1.0 , -8.0 , 1.0,
	    1.0, 1.0 , 1.0   
	);
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture1, TexCoord.st + offsets[i]));
    }
    vec3 col;
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    color = vec4(col, 1.0);
    */
}