#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cell
{
private:
	glm::vec2 position;
public:
	enum class cellType { blank, solid };
	cellType type;
	float waterLevel;

	Cell* top;
	Cell* bottom;
	Cell* left;
	Cell* right;
	Cell(glm::vec2 position, cellType type, Cell* top, Cell* bottom, Cell* left, Cell* right) {
		this->position = position;
		this->type = type;
		waterLevel = 0;

		this->top = top;
		this->bottom = bottom;
		this->left = left;
		this->right = right;
	}
	Cell() {
		this->type = cellType::blank;
		this->position = glm::vec4(0);
		waterLevel = 0;
		this->top = NULL;
		this->bottom = NULL;
		this->left = NULL;
		this->right = NULL;
	}
	bool isType(cellType type) {
		return this->type == type;
	}
};
