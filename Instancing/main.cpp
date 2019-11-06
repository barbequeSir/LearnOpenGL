#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SOIL.h>
// GLFW
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "LearnGL/Camera.h"
#include "LearnGL/Shader.h"
#include "LearnGL/Primitive.h"
#include "LearnGL/model.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void RenderScene(Shader & shader);
GLuint loadTexture(GLchar* path, GLboolean alpha = false);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 5.0, 50));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = NULL;
void Init();
void Render();
void UnInit();
void RenderScene();
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glCheckError();
	Init();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		Do_Movement();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Render();
	}
	UnInit();
	glfwTerminate();
	return 0;
}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_Q])
		camera.ProcessKeyboard(TOP, deltaTime);
	if (keys[GLFW_KEY_E])
		camera.ProcessKeyboard(BOTTOM, deltaTime);
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


GLuint loadTexture(GLchar* path, GLboolean alpha)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

GLuint loadCubeTexture()
{
	GLuint cubeTex;
	unsigned char* image;
	int w, h;
	vector<string> faces;
	faces.push_back("../LearnOpenGL/resources/textures/skybox/right.jpg");
	faces.push_back("../LearnOpenGL/resources/textures/skybox/left.jpg");
	faces.push_back("../LearnOpenGL/resources/textures/skybox/top.jpg");
	faces.push_back("../LearnOpenGL/resources/textures/skybox/bottom.jpg");
	faces.push_back("../LearnOpenGL/resources/textures/skybox/front.jpg");
	faces.push_back("../LearnOpenGL/resources/textures/skybox/back.jpg");
	glGenTextures(1, &cubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	for (int i = 0; i < 6; i++)
	{
		image = SOIL_load_image(faces[i].c_str(), &w, &h, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return cubeTex;
}

Model *planet = NULL;
Shader *planetShader = NULL;
GLuint planetDiffuse;

Model *rock = NULL;
Shader *rockShader = NULL;


glm::mat4* modelMatrices;
GLuint amount = 100000;
void UnInit()
{	
	delete planet;
	delete planetShader;

	delete rock;
	delete rockShader;
}
void Init()
{			
	GLint n;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
	
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed
	GLfloat radius = 50.0;
	GLfloat offset = 2.5f;
	for (GLuint i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat3(1.0);
		// 1. Translation: displace along circle with 'radius' in range [-offset, offset]
		GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y =  displacement * 0.4f; // y value has smaller displacement
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z =  cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// 2. Scale: Scale between 0.05 and 0.25f
		GLfloat scale =  (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		// 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
		GLfloat rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		// 4. Now add to list of matrices
		modelMatrices[i] = model;
	}


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	planet = new Model("../LearnOpenGL/resources/objects/planet/planet.obj");
	planetShader = new Shader("./planetShader.vs", "./planetShader.frag");	
	planetDiffuse = loadTexture("../LearnOpenGL/resources/objects/planet/planet_Quom1200.png");

	rock = new Model("../LearnOpenGL/resources/objects/rock/rock.obj");
	
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (int i = 0; i < rock->meshes.size(); i++)
	{
		glBindVertexArray(rock->meshes[i].VAO);
		
		GLuint size = sizeof(glm::vec4);
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid*)0);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid*)size);
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid*)(2*size));
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * size, (GLvoid*)(3*size));

		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);

		glBindVertexArray(0);
	}

	rockShader = new Shader("./rockShader.vs", "./rockShader.frag");	
}


void Render()
{
	RenderScene();
	glfwSwapBuffers(window);
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(camera.Zoom, float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0);

	planetShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(planetShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(planetShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(planetShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glBindTexture(GL_TEXTURE_2D, planet->textures_loaded[0].id);
	planet->Draw(*planetShader);

	rockShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(rockShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(rockShader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	glBindTexture(GL_TEXTURE_2D,rock->textures_loaded[0].id);
	/// draw  instanced
	///
	for (GLuint i = 0; i < rock->meshes.size(); i++)
	{
		glBindVertexArray(rock->meshes[i].VAO);
		//glDrawArrays(GL_TRIANGLES, 0, rock->meshes[i].vertices.size());		
		//glDrawArraysInstanced(GL_TRIANGLES, 0, rock->meshes[i].vertices.size(), amount);
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(),GL_UNSIGNED_INT,0,amount);
	}

	//draw each once
	/*for (int i = 0; i < amount; i++)
	{	
		glUniformMatrix4fv(glGetUniformLocation(rockShader->Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));
		rock->Draw(*rockShader);
	}*/
	glCheckError();
}

GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
