#pragma once
#include "Cell.h"
#include "SpriteRenderer.h"
class Grid
{
private:
	static const int gridWidth = 40;
	static const int gridHeight = 30;
	//4:3
	Cell grid[Grid::gridWidth][Grid::gridHeight];
	float maxWater = 1;
	float minWater = 0.003f;
	float change[Grid::gridWidth][Grid::gridHeight];
	glm::vec2 scrnResolution;
	glm::vec2 cellScale;
	SpriteRenderer solidRndr;
	SpriteRenderer waterRndr;
	Shader* shader;
	//4:3
	//0 = blank, 1 = solid
	int level[6][8]{
		{1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1}
	};

public:
	Grid(glm::vec2 screenResolution) {
		cellScale = glm::vec2(screenResolution.x / gridWidth, screenResolution.y / gridHeight);
		scrnResolution = screenResolution;
		for (int i = 0;i < gridWidth;i++) {
			for (int k = 0;k < gridHeight;k++) {
				//sample our "level" data for initial blocks
				int x = static_cast<int>(floor(((k / static_cast<double>(gridHeight)) * 6)));
				int y = static_cast<int>(floor(((i / static_cast<double>(gridWidth)) * 8)));
				//position is inverted
				glm::vec2 pos = glm::vec2((gridWidth - 1) - i, (gridHeight - 1) - k);
				grid[i][k] = Cell(pos,
					//level index is inverted because of position inversion
					level[5 - x][7 - y] == 1 ? Cell::cellType::solid : Cell::cellType::blank,
					k < gridHeight - 1 ? &grid[i][k + 1] : NULL,
					k > 0 ? &grid[i][k - 1] : NULL,
					i > 0 ? &grid[i - 1][k] : NULL,
					i < gridWidth - 1 ? &grid[i + 1][k] : NULL
				);
				//initialize water array
				change[i][k] = 0;
			}
		}

		glm::mat4 projection = glm::ortho(0.0f, screenResolution.x, screenResolution.y, 0.0f, -1.0f, 1.0f);
		shader = new Shader("Sprite.vert", "Sprite.frag");
		shader->Use();
		shader->SetMatrix4(projection, "projection");
		//set renderers for tiles
		solidRndr = SpriteRenderer(*shader, glm::vec4(0.5, 0.5, 0.5, 1));
		waterRndr = SpriteRenderer(*shader, glm::vec4(0.0, 0.5, 1, 1));
	}
	//convert from screen position to index of cell
	void getGridIndex(glm::vec2 scrnPos, int& xIndex, int& yIndex);
	//convert from index of cell to screen position
	glm::vec2 getSceenPos(glm::vec2 gridIndex);
	void AddWater(int waterLevel, glm::vec2 scrnPos);
	void SetTile(Cell::cellType type, glm::vec2 scrnPos);
	void Simulate();
	void Clear();
	~Grid() {
		delete shader;
	}
	void Draw();
};
