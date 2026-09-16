#include "BFS.h"

BFS::BFS() {}
bool BFS::solve(Grid& grid)
{
    steps = 0;
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    if (start == nullptr || goal == nullptr)
    {
        return false;
    }
    std::queue<Cell*> queue;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;
    // Bắt đầu
    queue.push(start);
    visited.insert(start);
    // BFS = 1
    start->markVisited(1);
    while (!queue.empty())
    {
        Cell* current = queue.front();
        queue.pop();
        steps++;
        // Tìm Goal
        if (current == goal)
        {
            break;
        }
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            if (visited.find(neighbor) ==
                visited.end())
            {
                visited.insert(neighbor);
                // BFS đi qua ô
                neighbor->markVisited(1);
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
    // Truy vết
    Cell* current = goal;
    while (current != start)
    {
        if (current != goal)
        {
            current->setState(
                CellState::PathBFS
            );
        }
        current = parent[current];
    }
    return true;
}
int BFS::getSteps() const {
    return steps; 
};