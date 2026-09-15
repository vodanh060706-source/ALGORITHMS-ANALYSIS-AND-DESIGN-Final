#include "Grid.h"
Grid::Grid(int rows, int cols, float cellSize)
    : rows(rows),cols(cols), offsetX(0.f), offsetY(0.f),
     cellSize(cellSize) {
        for(int row = 0; row < rows; row++) {
            for(int col = 0; col < cols; col ++) {
                cells.emplace_back(row, col, cellSize);
            }
        }
    }
int Grid::getRows() const{
    return rows;
 };
int Grid::getCols() const{
    return cols;
};
Cell* Grid::getStart() const {
    return startCell;
};
Cell* Grid::getGoal() const{
    return goalCell;
};

void Grid::draw(sf::RenderWindow &window) {
    for(Cell &cell : cells) {
        cell.draw(window);
    }
}
Cell* Grid::getCellAt(float x, float y) {
    for (Cell& cell : cells)
    {
        if (cell.contains(x, y))
        {
            return &cell;
        }
    }
    return nullptr;
}
Cell* Grid::getCell(int row, int col) {
    if (row < 0 || row >= getRows() ||
        col < 0 || col >= getCols())
    {
        return nullptr;
    }

    return &cells[row * cols + col];
};
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
void Grid::resize(int newRows, int newCols, float newCellSize)
{
    rows = newRows;
    cols = newCols;
    cellSize = newCellSize;

    offsetX = (960.f - cols * cellSize) / 2.f;
    offsetY = (720.f - rows * cellSize) / 2.f;

    cells.clear();
    cells.reserve(rows * cols);

    startCell = nullptr;
    goalCell = nullptr;

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            cells.emplace_back(row, col, cellSize);
            cells.back().setOffset(offsetX, offsetY);
        }
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
        { 0,1}, // Right
        { 0, -1}  // Left 
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