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

    // Xóa toàn bộ Grid về Wall
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            Cell* cell = grid.getCell(row, col);

            if (cell != nullptr)
                cell->setState(CellState::Wall);
        }
    }

    // Bắt đầu từ ô (1,1)
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
        {-2, 0},  // Up
        { 2, 0},  // Down
        { 0,-2},  // Left
        { 0, 2}   // Right
    };

    // Trộn hướng ngẫu nhiên
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::shuffle(directions.begin(), directions.end(), gen);

    for (const auto& direction : directions)
    {
        int newRow = row + direction.first;
        int newCol = col + direction.second;

        // Kiểm tra biên
        if (newRow <= 0 || newRow >= grid.getRows() - 1 ||
            newCol <= 0 || newCol >= grid.getCols() - 1)
        {
            continue;
        }

        Cell* next = grid.getCell(newRow, newCol);

        if (next == nullptr)
            continue;

        // Chỉ đi vào ô đang là Wall
        if (next->getState() == CellState::Wall)
        {
            // Phá Wall ở giữa 2 ô
            int wallRow = row + direction.first / 2;
            int wallCol = col + direction.second / 2;

            Cell* wall = grid.getCell(wallRow, wallCol);

            if (wall != nullptr)
                wall->setState(CellState::Empty);

            // Đệ quy
            carve(grid, newRow, newCol);
        }
    }
}