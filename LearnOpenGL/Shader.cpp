#include "Shader.h"



Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	string vCode, fCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		vShaderFile.close();
		fShaderFile.close();

		vCode = vShaderStream.str();
		fCode = fShaderStream.str();
		
	}
	catch (const std::exception&)
	{

	}

	const GLchar* vShaderCode = vCode.c_str();
	const GLchar* fShaderCode = fCode.c_str();

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{

	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{

	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{

	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}
