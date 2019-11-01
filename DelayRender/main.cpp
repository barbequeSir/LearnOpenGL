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
Camera camera(glm::vec3(0.0f, 5.0, 20.0));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLFWwindow* window = NULL;
void Init();
void Render();
void UnInit();
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
Model* model = NULL;
Shader* shaderNanosuit,*shaderLightPass,*shaderLightBox = NULL;
Shader* shaderPosition = NULL, *shaderNormal = NULL, *shaderDiffuse = NULL, *shaderSpecular = NULL;
GLuint fbo,rbo;
GLuint tex1, tex2, tex3;

const GLuint NR_LIGHTS = 32;
vector<glm::vec3> lightPositions;
vector<glm::vec3> lightColors;
vector<glm::vec3> objectPositions;
Primitive::Cube* objectCube = NULL;
void UnInit()
{
	delete model;
	delete shaderNanosuit;
	delete shaderPosition;
	delete shaderNormal;
	delete shaderDiffuse;
	delete shaderSpecular;
	delete shaderLightPass;
	delete shaderLightBox;
	delete objectCube;
}
void Init()
{	
	model = new Model("../LearnOpenGL/resources/objects/nanosuit/nanosuit.obj");
	shaderNanosuit =new Shader("./delay.vs", "./delay.frag");
	shaderPosition = new Shader("./delay.vs", "./position.frag");
	shaderNormal = new Shader("./delay.vs", "./normal.frag");
	shaderDiffuse = new Shader("./delay.vs", "./diffuse.frag");
	shaderSpecular = new Shader("./delay.vs", "./specular.frag");
	shaderLightPass = new Shader("./lightpass.vs", "./lightpass.frag");
	shaderLightBox = new Shader("./lightbox.vs", "./lightbox.frag");
	shaderLightPass->Use();
	objectCube = new Primitive::Cube();
	glUniform1i(glGetUniformLocation(shaderLightPass->Program, "gPosition"), 0);
	glUniform1i(glGetUniformLocation(shaderLightPass->Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(shaderLightPass->Program, "gAlbedoSpec"), 2);
#pragma region framebufferobject
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1, 0);

	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex2, 0);

	glGenTextures(1, &tex3);
	glBindTexture(GL_TEXTURE_2D, tex3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex3, 0);
	GLuint buffers[3] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, buffers);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region position color
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));
	srand(13);
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		// Calculate slightly random offsets
		GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// Also calculate random color
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
#pragma endregion
		
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
}
void RenderScene();
void RenderQuad();

void Render()
{	
	RenderScene();
	glfwSwapBuffers(window);
}

void RenderScene()
{	
	glm::mat4 projectMat = glm::perspective(camera.Zoom, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::mat4 viewMat = camera.GetViewMatrix();
	glm::mat4 modelMat = glm::mat4(1.0);

	glViewport(0, 0, WIDTH, HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderNanosuit->Use();
	glUniformMatrix4fv(glGetUniformLocation(shaderNanosuit->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderNanosuit->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	for (int i = 0; i < objectPositions.size(); i++)
	{
		modelMat = glm::mat4(1.0);
		//modelMat = glm::rotate(modelMat, (GLfloat)glfwGetTime() * 1.0f, glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
		modelMat = glm::translate(modelMat, objectPositions[i]);
		modelMat = glm::scale(modelMat, glm::vec3(0.3));
		glUniformMatrix4fv(glGetUniformLocation(shaderNanosuit->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
		model->Draw(*shaderNanosuit);
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderLightPass->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex3);	
	for (int i = 0; i < lightPositions.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(shaderLightPass->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
		glUniform3fv(glGetUniformLocation(shaderLightPass->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
		const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
		const GLfloat linear = 0.7;
		const GLfloat quadratic = 1.8;
		glUniform1f(glGetUniformLocation(shaderLightPass->Program, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(shaderLightPass->Program, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
		glUniform1f(glGetUniformLocation(shaderLightPass->Program, ("lights[" + std::to_string(i) + "].Radis").c_str()), 55.0);
	}
	glUniform3fv(glGetUniformLocation(shaderLightPass->Program, "viewPos"), 1, &camera.Position[0]);
	RenderQuad();	
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);	
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	
	glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	shaderLightBox->Use();
	glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	
	for (int i = 0; i < lightPositions.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(shaderLightBox->Program, "lightColor"), 1, &lightColors[i][0]);
		modelMat = glm::mat4(1.0);		
		modelMat = glm::translate(modelMat, lightPositions[i]);				
		modelMat = glm::scale(modelMat, glm::vec3(0.1));
		glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
		objectCube->Draw(*shaderLightBox);
	}	
	
#pragma region MyRegion
	glViewport(0, HEIGHT / 6, WIDTH / 4, HEIGHT / 3);
	shaderPosition->Use();
	modelMat = glm::mat4(1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderPosition->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderPosition->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderPosition->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
	model->Draw(*shaderPosition);

	glViewport(WIDTH * 1 / 4, HEIGHT / 6, WIDTH / 4, HEIGHT / 3);
	shaderNormal->Use();
	glUniformMatrix4fv(glGetUniformLocation(shaderNormal->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderNormal->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderNormal->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
	model->Draw(*shaderNormal);

	glViewport(WIDTH * 2 / 4, HEIGHT / 6, WIDTH / 4, HEIGHT / 3);
	shaderDiffuse->Use();
	glUniformMatrix4fv(glGetUniformLocation(shaderDiffuse->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderDiffuse->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderDiffuse->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
	model->Draw(*shaderDiffuse);

	glViewport(WIDTH * 3 / 4, HEIGHT / 6, WIDTH / 4, HEIGHT / 3);
	shaderSpecular->Use();
	glUniformMatrix4fv(glGetUniformLocation(shaderSpecular->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderSpecular->Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderSpecular->Program, "transform"), 1, GL_FALSE, glm::value_ptr(modelMat));
	model->Draw(*shaderSpecular);
#pragma endregion

	
}

GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			// Positions        // Texture Coords
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
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
