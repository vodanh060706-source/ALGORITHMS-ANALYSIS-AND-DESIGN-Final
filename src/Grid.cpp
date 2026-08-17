#include "Grid.h"
 int Grid::getRows() const{
    return rows;
 };
int Grid::getCols() const{
    return cols;
};
Grid::Grid(int rows, int cols, float cellSize)
    : rows(rows),cols(cols), cellSize(cellSize) {
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
std::vector<Cell*> Grid::getNeighbors(Cell* cell) {
    std::vector<Cell*> neighbors;
    if(cell == nullptr) {
        return neighbors;
    }

    int row = cell->getRow();
    int col = cell->getCol();

     int directions[4][2] =
    {
        {-1, 0}, // Up
        {1, 0}, // Down
        { 0,1}, // Left
        { 0, -1}  // Right
    };
    // Xác định hướng
    for(int i=0; i<4; i++) {
        int newRow = row + directions[i][0];
        int newCol  = col + directions[i][1];
        // Kiểm tra có trong phạm vi khung không
        if(newRow < 0 || newRow >= rows || 
            newCol < 0 || newCol >= cols) continue;
         Cell* neighbor = &cells[newRow * cols + newCol];
        // Không đi qua Wall
        if (neighbor->getState() != CellState::Wall)
        {
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}