#include "Grid.h"

void Grid::getGridIndex(glm::vec2 scrnPos, int& xIndex, int& yIndex)
{
	//top left corner of screen needs to be [0][0], bottom right needs to be [gridWidth][gridHieght]
	xIndex = round((scrnResolution.x - scrnPos.x) / cellScale.x);
	yIndex = round((scrnResolution.y - scrnPos.y) / cellScale.y);
}

glm::vec2 Grid::getSceenPos(glm::vec2 gridIndex)
{
	glm::vec2 scrnPos(
		scrnResolution.x - floor((cellScale.x * gridIndex.x) + cellScale.x),
		scrnResolution.y - floor((cellScale.y * gridIndex.y) + cellScale.y)
	);
	return scrnPos;
}

void Grid::AddWater(int waterLevel, glm::vec2 scrnPos)
{
	int xIndex, yIndex;
	getGridIndex(scrnPos, xIndex, yIndex);

	Cell& cell = grid[xIndex - 1][yIndex - 1];
	if (cell.type == Cell::cellType::blank && cell.waterLevel < maxWater) {
		cell.waterLevel += waterLevel;
	}
}

void Grid::SetTile(Cell::cellType type, glm::vec2 scrnPos)
{
	int xIndex, yIndex;
	getGridIndex(scrnPos, xIndex, yIndex);
	Cell& cell = grid[xIndex - 1][yIndex - 1];
	cell.type = type;
	cell.waterLevel = 0;
}

void Grid::Simulate()
{
	for (int x = 0;x < gridWidth;x++) {
		for (int y = 0;y < gridHeight;y++) {
			Cell& cell = grid[x][y];
			if (cell.isType(Cell::cellType::blank)) {
				float amount = cell.waterLevel;
				float flow = 0;
				if (cell.bottom && cell.bottom->isType(Cell::cellType::blank)) {
					if (cell.bottom->waterLevel < maxWater && y > 0) {
						if (cell.bottom->waterLevel < maxWater) {
							if (cell.bottom->waterLevel + amount < maxWater) {
								flow = amount;
							}
							else {
								flow = maxWater - cell.bottom->waterLevel;
							}
						}
						if (flow != 0) {
							amount -= flow;
							change[x][y] -= flow;
							change[x][y - 1] += flow;
						}
					}
				}
				if (cell.left && cell.left->isType(Cell::cellType::blank) && x > 0) {

					flow = (amount - cell.left->waterLevel) / 4.0f;
					flow = glm::max(flow, 0.0f);
					if (flow != 0) {
						amount -= flow;
						change[x][y] -= flow;
						change[x - 1][y] += flow;
					}
					if (amount < minWater) {
						change[x][y] -= amount;
						continue;
					}
				}
				if (cell.right && cell.right->isType(Cell::cellType::blank)) {

					flow = (amount - cell.right->waterLevel) / 4.0f;
					flow = glm::max(flow, 0.0f);
					if (flow != 0) {
						amount -= flow;
						change[x][y] -= flow;
						change[x + 1][y] += flow;
					}
					if (amount < minWater) {
						change[x][y] -= amount;
						continue;
					}
				}
			}
		}
	}
	for (int i = 0;i < gridWidth;i++) {
		for (int k = 0;k < gridHeight;k++) {
			Cell& cell = grid[i][k];
			cell.waterLevel += change[i][k];
			change[i][k] = 0;
		}
	}
}

void Grid::Clear()
{
	for (int x = 0;x < gridWidth;x++) {
		for (int y = 0;y < gridHeight;y++) {
			Cell& cell = grid[x][y];
			cell.waterLevel = 0;
		}
	}
}

void Grid::Draw()
{
	for (int i = 0;i < gridWidth;i++) {
		for (int k = 0;k < gridHeight;k++) {
			glm::vec2 scrnPos = getSceenPos(glm::vec2(i, k));
			Cell& cell = grid[i][k];
			if (grid[i][k].isType(Cell::cellType::solid)) {
				solidRndr.Draw(scrnPos, cellScale / 2.0f);
			}
			else if (cell.waterLevel > 0) {
				glm::vec2 scale = cellScale / 2.0f;
				glm::vec2 pos = scrnPos;

				if ((!cell.top || cell.top && cell.top->waterLevel == 0)) {

					float waterScale = glm::clamp(grid[i][k].waterLevel, 0.0f, 1.0f);
					scale.y *= waterScale;
					//positive screen pos y = downward
					pos.y += ((1.0 - waterScale) * cellScale.y) / 2.0;
				}

				waterRndr.Draw(pos, scale);
			}
		}
	}
}
