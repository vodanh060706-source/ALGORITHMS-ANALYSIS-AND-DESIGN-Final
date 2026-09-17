#include "MazeGenerator.h"

#include <algorithm>
#include <random>
#include <vector>

void MazeGenerator::generate(Grid& grid)
{
    int rows = grid.getRows();
    int cols = grid.getCols();

    if (rows < 3 || cols < 3)
        return;
    // 1. Đầu tiên biến toàn bộ ô thành WALL
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            Cell* cell = grid.getCell(row, col);

            if (cell != nullptr)
            {
                cell->setState(CellState::Wall);
            }
        }
    }
    // 2. Bắt đầu tạo đường đi từ (1,1)
    carve(grid, 1, 1);
}

void MazeGenerator::carve(Grid& grid, int row, int col)
{
    Cell* current = grid.getCell(row, col);

    if (current == nullptr)
        return;

    // Ô hiện tại trở thành đường đi
    current->setState(CellState::Empty);
    std::vector<std::pair<int, int>> directions =
    {
        {-2, 0},   // Up
        { 2, 0},   // Down
        { 0,-2},   // Left
        { 0, 2}    // Right
    };

    // Random hướng
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::shuffle(
        directions.begin(),
        directions.end(),
        gen
    );

    for (const auto& direction : directions)
    {
        int newRow = row + direction.first;
        int newCol = col + direction.second;

        // Không đi sát biên
        if (newRow <= 0 ||
            newRow >= grid.getRows() - 1 ||
            newCol <= 0 ||
            newCol >= grid.getCols() - 1)
        {
            continue;
        }

        Cell* next = grid.getCell(newRow, newCol);

        if (next == nullptr)
            continue;

        // Chỉ đi vào ô Wall chưa phá
        if (next->getState() == CellState::Wall)
        {
            // Ô tường nằm giữa
            int wallRow =
                row + direction.first / 2;

            int wallCol =
                col + direction.second / 2;

            Cell* wall =
                grid.getCell(wallRow, wallCol);

            if (wall != nullptr)
            {
                wall->setState(CellState::Empty);
            }
            // Tiếp tục DFS
            carve(
                grid,
                newRow,
                newCol
            );
        }
    }
}