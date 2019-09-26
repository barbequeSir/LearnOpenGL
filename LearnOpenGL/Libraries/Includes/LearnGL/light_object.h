#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GL\glew.h>
#include<vector>
#include"Shader.h"
using namespace std;
struct Vertex_PV {
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
};

class Material {
public:

	
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::float32 shiness;

	Material(){}

	Material(glm::vec3 ia, glm::vec3 id, glm::vec3 is, glm::float32 ishine)
	{
		ambient = ia;
		diffuse = id;
		specular = is;
		shiness = ishine;
	}
};
class DirectionLight {
public:
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 position;
	glm::vec3 direction;
	Material material;
	DirectionLight(){}
	DirectionLight(glm::vec3 p, glm::vec3 d, Material m)
	{
		position = p;
		direction = d;
		material = m;

		vector<Vertex_PV> vertexs;

		vector<GLfloat> vers = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
		};

		//glm::mat4 lightView = glm::lookAt(this->position, this->position + this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightView = viewMat();
		for (int i = 0; i < 36; i++)
		{
			Vertex_PV v;
			glm::vec4 p = lightView * glm::vec4(vers[i * 8], vers[i * 8 + 1], vers[i * 8 + 2], 1.0);
			v.Position.x = p.x;
			v.Position.y = p.y;
			v.Position.z = p.z;			

			v.Normal.x = vers[i * 8 + 3];
			v.Normal.y = vers[i * 8 + 4];
			v.Normal.z = vers[i * 8 + 5];
			v.TexCoords.x = vers[i * 8 + 6];
			v.TexCoords.y = vers[i * 8 + 7];	
			vertexs.push_back(v);
		}


		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(Vertex_PV), &vertexs[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PV), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_PV), (void*)(offsetof(Vertex_PV, Normal)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_PV), (void*)(offsetof(Vertex_PV, TexCoords)));
	}
	
	void Draw(Shader program)
	{	
		glBindVertexArray(this->vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
private :
	GLuint vao, vbo, ibo;

	glm::mat4 viewMat()
	{
		glm::vec3 f = glm::normalize(this->direction);
		glm::vec3 r = glm::normalize( glm::cross(this->up, this->direction));
		glm::vec3 u = glm::normalize(glm::cross(f, r));
		glm::mat4 m(glm::vec4(r,1.0f),glm::vec4(u,1.0f),glm::vec4(f,1.0f),glm::vec4(this->position,1.0));
		return m;
	}
};