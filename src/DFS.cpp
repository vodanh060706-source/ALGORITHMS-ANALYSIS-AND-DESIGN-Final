#include "DFS.h"

DFS::DFS() { }

bool DFS::solve(Grid& grid) {
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    // Kiểm tra xem đích và khởi đầu có tồn tại không 
    if (start == nullptr || goal == nullptr) {
        return false;
    }

    std::stack<Cell*> stack;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;
    // Thêm vào ô khởi đầu và đánh dấu ô đã thăm 
    stack.push(start);
    visited.insert(start);
    // Duyệt đến chừng nào hết ô khám phá 
    bool found = false;
    while (!stack.empty())
    {
        Cell* current = stack.top();
        stack.pop();

        if (current == goal)
        {
            found = true;
            break;
        }

        std::vector<Cell*> neighbors = grid.getNeighbors(current);

        for (Cell* neighbor : neighbors)
        {
            if (visited.find(neighbor) == visited.end())
            {
                visited.insert(neighbor);
                parent[neighbor] = current;
                stack.push(neighbor);
            }
        }
    }
    // Không tìm thấy Goal 
    if(!found) {
        return false;
    }
    // Truy vết đường đi 
    Cell* current = goal;
    while(current != start) {
        current->setState(CellState::PathDFS);
        current = parent[current];
    }
    return true;
}