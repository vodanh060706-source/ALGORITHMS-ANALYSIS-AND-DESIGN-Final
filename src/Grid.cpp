#include "Grid.h"

Grid::Grid(int rows, int cols, float cellSize)
    : rows(rows), cols(cols), cellSize(cellSize) {
        for(int row = 0; row < rows; row++) {
            for(int col = 0; col < cols; col ++) {
                cells.emplace_back(row, col, cellSize);
            }
        }
    }
void Grid::draw(sf::RenderWindow &window) {
    for(Cell &cell : cells) {
        cell.draw(window);
    }
}
Cell* Grid::getCellAt(float x, float y) {
    for (Cell& cell : cells) {
        if(cell.contains(x,y)){
            return &cell;
        }
    }
    return nullptr;
}
void Grid::setStart(Cell *cell) {
    // Xóa start cũ
    if (startCell != nullptr) {
        startCell ->setState(CellState::Empty);
    };
    // Đặt lại cell start mới 
    startCell = cell;
    if(startCell != nullptr) {
        startCell->setState(CellState::Start);
    }
}

void Grid::setGoal(Cell *cell) {
    // Xóa start cũ
    if (goalCell != nullptr) {
        goalCell ->setState(CellState::Empty);
    };
    // Đặt lại cell start mới 
    goalCell = cell;
    if(goalCell != nullptr) {
        goalCell->setState(CellState::Goal);
    }
}