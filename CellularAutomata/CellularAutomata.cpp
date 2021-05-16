#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include <string>
#include <fstream>
//#include "Shader.h"
//#include "SpriteRenderer.h"
#include "Grid.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void UserInput(Grid *grid, GLFWwindow *window) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		glm::vec2 scrnPos = glm::vec2(xPos, yPos);
		grid->SetTile(Cell::cellType::blank, scrnPos);
		grid->AddWater(1, scrnPos);
	}
	int state2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state2 == GLFW_PRESS)
	{
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		glm::vec2 scrnPos = glm::vec2(xPos, yPos);
		grid->SetTile(Cell::cellType::solid, scrnPos);
	}
	int key1 = glfwGetKey(window, GLFW_KEY_R);
	if (key1 == GLFW_PRESS)
	{
		grid->Clear();
	}
}
int main()
{
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "Water", NULL, NULL);
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
	Grid  *grid = new Grid(glm::vec2(800,600));
	double lastFrameTime = glfwGetTime();
	double delta;
	double simulateTimer = 0;
	double simulateWaitTime = 0.05;
	//core loop
	while (!glfwWindowShouldClose(window))
	{
		//sets clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//clears screen
		glClear(GL_COLOR_BUFFER_BIT);
		grid->Draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
		UserInput(grid,window);
		delta = glfwGetTime() - lastFrameTime;
		simulateTimer += delta;
		if (simulateTimer > simulateWaitTime) {
			simulateTimer = 0;
			grid->Simulate();
		}
		lastFrameTime = glfwGetTime();
	}

	delete sh;
	delete grid;

	glfwTerminate();
	return 0;
}
