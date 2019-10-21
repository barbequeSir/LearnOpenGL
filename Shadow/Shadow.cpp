#include "stdafx.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SOIL.h>
// GLFW
#include <GLFW/glfw3.h>
#include<SOIL.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "LearnGL/Camera.h"
#include "LearnGL/Shader.h"
#include "LearnGL/Primitive.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(GLchar* path, GLboolean alpha = false);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 5.0f, 5.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = NULL;
void Init();
void Render();
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
vector<GLfloat> floorVertexs = {
	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
	-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

	25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
	25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
	-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
};
vector<GLfloat> cubeVertexs = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
    // Left face
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
    // Right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        

};

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

GLuint tex_diff;
GLuint floorVao, cubeVao;
Shader floorShader, cubeShader,defaultCubeShader;
Primitive::Cube CubeObj;
void Init()
{	
	tex_diff = loadTexture("../LearnOpenGL/resources/textures/bricks2.jpg");
	floorVao = Primitive::CubeP3N3T2(floorVertexs);
	cubeVao = Primitive::CubeP3N3T2(cubeVertexs);
	floorShader = Shader("./Shader/floor.vs", "./Shader/floor.frag");
	cubeShader = Shader("./Shader/cube.vs", "./Shader/cube.frag");
	defaultCubeShader = Shader("../LearnOpenGL/Shader/cubeP3N3T2.vs", "../LearnOpenGL/Shader/cubeP3N3T2.frag");

	CubeObj = Primitive::Cube(glm::vec3(-10.0f, 10.0f, -0.0f), 0.1f);
}

void Render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glm::mat4 proj = glm::perspective(90.0f, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 modelFloor = glm::mat4(1.0f);
	modelFloor = glm::translate(modelFloor, glm::vec3(0, 0, -10));
	floorShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "proj"),1,GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFloor));
	glUniform3f(glGetUniformLocation(floorShader.Program, "LightPos"), CubeObj.m_position.x, CubeObj.m_position.y, CubeObj.m_position.z);
	glUniform3f(glGetUniformLocation(floorShader.Program, "CameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glBindVertexArray(floorVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	cubeShader.Use();
	glm::mat4 modelCube = glm::mat4(1.0f);
	modelCube = glm::translate(modelCube, glm::vec3(0, 2, 0));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCube));
	glUniform3f(glGetUniformLocation(cubeShader.Program, "LightPos"), CubeObj.m_position.x, CubeObj.m_position.y, CubeObj.m_position.z);
	glUniform3f(glGetUniformLocation(cubeShader.Program, "CameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	glBindVertexArray(cubeVao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glBindVertexArray(0);
	defaultCubeShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(defaultCubeShader.Program, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(defaultCubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	CubeObj.Draw(defaultCubeShader);

	glfwSwapBuffers(window);
}