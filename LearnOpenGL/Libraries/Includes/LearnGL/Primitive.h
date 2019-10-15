#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
struct P3
{
	glm::vec3 position;
};
struct P3N3
{
	glm::vec3 position;
	glm::vec3 normal;
};
class Primitive
{	
public:
	Primitive(){}
	static GLuint CubeP3N3(vector<GLfloat> verts)
	{		
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size()  * sizeof(P3N3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		return vao;
	}

	static GLuint CubeP3(vector<GLfloat> verts)
	{	
		GLuint vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(P3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		return vao;
	}
	

};