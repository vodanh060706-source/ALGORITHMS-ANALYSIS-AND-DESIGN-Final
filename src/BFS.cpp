#include "BFS.h"
BFS::BFS() {}
bool BFS::solve(Grid &grid) {
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();

    if (start == nullptr || goal == nullptr)
    {
        return false;
    }

    std::queue<Cell*> queue;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;

    queue.push(start);
    visited.insert(start);

    while (!queue.empty())
    {
        Cell* current = queue.front();
        queue.pop();

        // Đã tìm thấy Goal
        if (current == goal)
        {
            break;
        }

        // Lấy 4 ô hàng xóm
        std::vector<Cell*> neighbors = grid.getNeighbors(current);

        for (Cell* neighbor : neighbors)
        {
            // Chưa đi qua
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);

                // Lưu ô cha
                parent[neighbor] = current;

                queue.push(neighbor);
            }
        }
    }

    // Không tìm thấy Goal
    if (visited.find(goal) == visited.end())
    {
        return false;
    }

    // Truy ngược đường đi
    Cell* current = goal;

    while (current != start)
    {
        current->setState(CellState::PathBFS);
        current = parent[current];
    }

    return true;
}