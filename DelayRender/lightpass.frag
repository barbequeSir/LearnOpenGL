#version 330 core
out vec4 color;
in vec2 Texcoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
struct Light{
	vec3 Position;
	vec3 Color;
	float Linear;
	float Quadratic;
	float Radis;	
};
const int NR_LIGHT = 32;
uniform Light lights[NR_LIGHT];
uniform vec3 viewPos;
void main()
{   	
	vec3 worldPos = texture(gPosition,Texcoords).rgb;
	vec3 normal = texture(gNormal,Texcoords).rgb;
	vec3 albedo = texture(gAlbedoSpec,Texcoords).rgb ;
	float specular = texture(gAlbedoSpec,Texcoords).a;
	
	vec3 viewDir = normalize(viewPos - worldPos);

	vec3 lighting = albedo * 0.3;
	
	for(int i = 0;i<NR_LIGHT;i++)
	{
		float distance=  length(lights[i].Position - worldPos);
		if(distance < lights[i].Radis)
		{
			vec3 lightDir = normalize(lights[i].Position - worldPos);
			float diff = max(0.0,dot(lightDir,normal));
			vec3 diffColor = albedo * lights[i].Color * diff;
			vec3 half = normalize(viewDir + lightDir);
			float spec = pow(max(0.0,dot(half,normal)),16.0);
			vec3 specColor = specular * lights[i].Color * spec;
			
			float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
            diffColor *= attenuation;
            specColor *= attenuation;
			lighting = lighting + diffColor +specColor;
		}
		
	}

	color = vec4(lighting ,1.0);
}