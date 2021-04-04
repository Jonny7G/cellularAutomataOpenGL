#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

class SpriteRenderer
{
private:
	Shader shader;
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
public:
	SpriteRenderer(Shader shdr) {

		this->shader = shdr;
		//vertex data
		float vertices[] = {
		 1,  1,  // top right
		 1, -1,  // bottom right
		 -1, -1,  // bottom left
		 -1,  1   // top left 
		};
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		//Vertex Buffer Object
		unsigned int VBO;
		//Vertex Attribute Object
		unsigned int VAO;
		//Element Buffer Object
		unsigned int EBO;
		//generate one vertex attribute object
		glGenVertexArrays(1, &VAO);
		//generate one vertex buffer object
		glGenBuffers(1, &VBO);
		//generate one Element Buffer Object
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
		glBindVertexArray(VAO);
		//bind VBO as the GL_ARRAY_BUFFER
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//set the data of GL_ARRAY_BUFFER to our vertex data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//bind EBO as the GL_ELEMENT_ARRAY_BUFFER
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//set GL_ELEMENT_ARRAY_BUFFER data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//configure vertex attribute data. 0 = vertex attribute we are configuring (location = 0), 3 = size of attribute, type of data GL_FLOAT, specificy if the values should be normalized, stride (space between data), offset
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		//enable vertex attribute
		glEnableVertexAttribArray(0);
		//unbind VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//unbind VAO
		glBindVertexArray(0);

		this->VAO = VAO;
		this->VBO = VBO;
		this->EBO = EBO;
	}
	void Draw(glm::vec2 position, glm::vec2 size) {
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));
		model = glm::scale(model, glm::vec3(size, 1.0f));
		this->shader.Use();
		this->shader.SetMatrix4(model, "model");

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};
