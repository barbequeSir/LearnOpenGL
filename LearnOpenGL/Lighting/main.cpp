#include <iostream>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>
#include<SOIL.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include"LearnGL/Shader.h"
#include "LearnGL/Camera.h"
#include "LearnGL\model.h"

#include"LearnGL/light_object.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;	
	glfwInit();	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);	
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);	
	glewExperimental = GL_TRUE;	
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	//Model model("../resources/objects/nanosuit/nanosuit.obj");
	Shader ourShader("../Shader/nanosuit.vs", "../Shader/nanosuit.frag");
	Shader floorShader("../Shader/floor.vs", "../Shader/floor.frag");
	GLfloat floorVertexs[] = {
		//0.5f, 0.5f, 0.0f,   // 右上角
		//0.5f, -0.5f, 0.0f,  // 右下角
		//-0.5f, -0.5f, 0.0f, // 左下角
		//-0.5f, 0.5f, 0.0f   // 左上角
		-1.0f,-0.5f,1.0f,
		1.0f,-0.5f,1.0f,
		1.0f,-0.5f,-1.0f,
		-1.0f,-0.5f,-1.0f
	};

	GLuint floorIndex[] = {
		//0, 1, 3, // 第一个三角形
		//1, 2, 3  // 第二个三角形
		0,1,3,
		1,2,3
	};

	GLuint eboFloor,eboScreen, vboFloor, vboScreen, vaoFloor, vaoScreen;
	glGenVertexArrays(1, &vaoFloor);
	glBindVertexArray(vaoFloor);
	glGenBuffers(1, &vboFloor);
	glBindBuffer(GL_ARRAY_BUFFER, vboFloor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertexs),floorVertexs, GL_STATIC_DRAW);	
	glGenBuffers(1, &eboFloor);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboFloor);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndex), floorIndex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);	
	glBindVertexArray(0);
		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		Do_Movement();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);		
		glm::mat4 projectionMat = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
		glm::mat4 viewMat = camera.GetViewMatrix();
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::scale(modelMat, glm::vec3(0.20f));
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -1.0f, -50.0f));
		/*ourShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));*/		
		//model.Draw(ourShader);
		floorShader.Use();
		modelMat = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(floorShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glBindVertexArray(vaoFloor);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
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