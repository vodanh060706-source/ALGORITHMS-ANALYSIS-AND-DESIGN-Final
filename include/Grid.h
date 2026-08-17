#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"

class Grid {
    private: 
    int rows;
    int cols;
    float cellSize;
    std::vector<Cell> cells;
    Cell *startCell = nullptr;
    Cell *goalCell = nullptr;
    public:
    Grid(int rows, int cols, float cellSize);
    void draw(sf::RenderWindow &window);
    Cell *getCellAt(float x, float y);
    void setStart(Cell *cell);
    void setGoal(Cell *cell);
    int getRows() const;
    int getCols() const;
    std::vector<Cell*> getNeighbors(Cell* cell);
};