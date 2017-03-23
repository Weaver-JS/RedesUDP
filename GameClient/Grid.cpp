#include "Grid.h"



void Grid::setCharacter(int x, int y)
{
	grid[x][y] = 1;
}

void Grid::setCharacter(sf::Vector2<int> v)
{
	grid[v.x][v.y] = 1;
}

void Grid::removeCharacter(int x, int y)
{
	grid[x][y] = 0;
}

void Grid::removeCharacter(sf::Vector2<int> v)
{
	grid[v.x][v.y] = 0;
}

Grid::Grid()
{
}


Grid::~Grid()
{
}
