#include "DFS.h"

DFS::DFS()
{}
int DFS::getSteps() const
{
    return steps;
}
bool DFS::solve(Grid& grid)
{
    steps = 0;
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    if (start == nullptr || goal == nullptr)
    {
        return false;
    }
    std::stack<Cell*> stack;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;
    // Đưa Start vào stack
    stack.push(start);
    visited.insert(start);
    // DFS = 2
    start->markVisited(2);
    bool found = false;
    while (!stack.empty())
    {
        Cell* current = stack.top();
        stack.pop();
        steps++;
        // Đã tìm thấy Goal
        if (current == goal)
        {
            found = true;
            break;
        }
        // Lấy các ô hàng xóm
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                // Đánh dấu DFS đã đi qua
                // Nếu ô đã được BFS đi qua → Overlap
                neighbor->markVisited(2);
                // Lưu ô cha
                parent[neighbor] = current;
                // Đưa vào stack
                stack.push(neighbor);
            }
        }
    }
    // Không tìm thấy Goal
    if (!found)
    {
        return false;
    }
    // Truy vết đường đi
    Cell* current = goal;
    while (current != start)
    {
        if (current != goal)
        {
            current->setState(CellState::PathDFS);
        }

        current = parent[current];
    }
    return true;
}