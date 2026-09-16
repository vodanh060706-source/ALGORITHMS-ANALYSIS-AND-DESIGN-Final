#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"

class Grid {
    private: 
    int rows;
    int cols;
    float offsetX;
    float offsetY;
    float cellSize;
    std::vector<Cell> cells;
    Cell *startCell = nullptr;
    Cell *goalCell = nullptr;
    public:
    Grid(int rows, int cols, float cellSize);
    void draw(sf::RenderWindow &window);
    Cell *getCellAt(float x, float y);
    Cell* getCell(int row, int col);
    void setStart(Cell *cell);
    void setGoal(Cell *cell);
    int getRows() const;
    int getCols() const;
    Cell* getStart() const;
    Cell* getGoal() const;
    void setOffset(float x, float y);
    void resize(int rows, int cols, float cellSize);
    void clearPath();
    std::vector<Cell*> getNeighbors(Cell* cell);
};