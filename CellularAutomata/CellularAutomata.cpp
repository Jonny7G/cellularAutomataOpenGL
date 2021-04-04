#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <string>
#include <fstream>
#include "Shader.h"
#include "SpriteRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

	Shader *sh = new Shader("Sprite.vert", "Sprite.frag");
	sh->Use();
	sh->SetMatrix4(projection, "projection");

	SpriteRenderer sr(*sh);
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//sets clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//clears screen
		glClear(GL_COLOR_BUFFER_BIT);
		sr.Draw(glm::vec2(0.0f, 0.0f),glm::vec2(35.0f, 35.0f));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete sh;

	glfwTerminate();
	return 0;
}
