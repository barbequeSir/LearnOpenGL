#pragma once

#include<GL/glew.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

using namespace std;
class Shader
{
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void Use();
};

