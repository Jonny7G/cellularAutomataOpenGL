#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include <string>
#include <fstream>

class Shader {
private:
	unsigned int program;

public:
	Shader() {
		program = 0;
	}
	Shader(std::string vertPath, std::string fragPath) {
		std::ifstream vFile;
		vFile.open(vertPath);
		std::ifstream fFile;
		fFile.open(fragPath);

		std::string vertStr = std::string((std::istreambuf_iterator<char>(vFile)),
			(std::istreambuf_iterator<char>()));
		const char* vertSource = vertStr.c_str();

		std::string fragStr = std::string((std::istreambuf_iterator<char>(fFile)),
			(std::istreambuf_iterator<char>()));
		const char* fragSource = fragStr.c_str();

		//vert shader
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertSource, NULL);
		glCompileShader(vertexShader);

		//frag shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragSource, NULL);
		glCompileShader(fragmentShader);

		//shader program

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		int success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}
	}
	void SetMatrix4(glm::mat4 mat,std::string name) {
		glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()),1,false, glm::value_ptr(mat)); //set mat4 in shader
	}
	void SetVec4(glm::vec4 vec, std::string name) {
		glUniform4fv(glGetUniformLocation(program, name.c_str()), 1,glm::value_ptr(vec));
	}
	void Use() {
		glUseProgram(program);
	}
};

