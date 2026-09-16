#include "DFS.h"

DFS::DFS() {}
int DFS::getPathLength() const
{
    return pathLength;
}
double DFS::getExecutionTime() const
{
    return executionTime;
}
// Truy vết đường đi 
void DFS::reconstructPath()
{
    pathLength = 0;
    Cell* current = goal;
    if (current == nullptr || start == nullptr)
        return;
    while (current != nullptr && current != start)
    {
        pathLength++;
        // Không đổi màu Goal
        if (current != goal)
        {
            current->setState(CellState::PathDFS);
        }
        auto it = parent.find(current);
        if (it == parent.end())
            break;
        current = it->second;
    }
    // Tính cả Start
    if (current == start)
    {
        pathLength++;
    }
}
// Khởi tạo DFS 
void DFS::startAlgorithm(Grid& grid)
{
    this->grid = &grid;
    // Xóa dữ liệu DFS cũ
    stack = std::stack<Cell*>();
    visited.clear();
    parent.clear();
    start = grid.getStart();
    goal = grid.getGoal();
    finished = false;
    found = false;
    steps = 0;
    pathLength = 0;
    executionTime = 0.0;
    // Không có Start hoặc Goal
    if (start == nullptr || goal == nullptr)
    {
        finished = true;
        return;
    }
    animationStartTime = std::chrono::high_resolution_clock::now();
    // Đưa Start vào stack
    stack.push(start);
    visited.insert(start);
    // DFS = 2
    start->markVisited(2);
}
// Thực hiện từng bước DFS 
bool DFS::step()
{
    if (finished)
        return false;

    if (grid == nullptr ||
        start == nullptr ||
        goal == nullptr)
    {
        finished = true;

        auto endTime =
            std::chrono::high_resolution_clock::now();

        executionTime =
            std::chrono::duration<double, std::milli>(
                endTime - animationStartTime
            ).count();

        return false;
    }

    if (stack.empty())
    {
        finished = true;
        found = false;

        auto endTime =
            std::chrono::high_resolution_clock::now();

        executionTime =
            std::chrono::duration<double, std::milli>(
                endTime - animationStartTime
            ).count();

        return false;
    }

    Cell* current = stack.top();
    stack.pop();

    steps++;

    if (current == goal)
    {
        found = true;
        reconstructPath();
        finished = true;

        auto endTime =
            std::chrono::high_resolution_clock::now();

        executionTime =
            std::chrono::duration<double, std::milli>(
                endTime - animationStartTime
            ).count();

        return false;
    }

    std::vector<Cell*> neighbors =
        grid->getNeighbors(current);

    for (Cell* neighbor : neighbors)
    {
        if (visited.find(neighbor) == visited.end())
        {
            visited.insert(neighbor);

            neighbor->markVisited(2);

            parent[neighbor] = current;

            stack.push(neighbor);
        }
    }

    return true;
}
bool DFS::isFinished() const
{
    return finished;
}
bool DFS::isFound() const
{
    return found;
}
int DFS::getSteps() const
{
    return steps;
}
// DFS Chạy toàn bộ, không animation 
bool DFS::solve(Grid& grid)
{
    steps = 0;
    pathLength = 0;
    executionTime = 0.0;
    auto startTime = std::chrono::high_resolution_clock::now();
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    if (start == nullptr || goal == nullptr)
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
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
                neighbor->markVisited(2);
                // Lưu ô cha
                parent[neighbor] = current;
                // Đưa vào stack
                stack.push(neighbor);
            }
        }
    }
    // Kết thúc thời gian
    auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    // Không tìm thấy Goal
    if (!found)
    {
        return false;
    }
    // Truy vết đường đi 
    Cell* current = goal;
    while (current != nullptr && current != start)
    {
        pathLength++;
        // Không đổi màu Goal
        if (current != goal)
        {
            current->setState(CellState::PathDFS);
        }
        auto it = parent.find(current);
        if (it == parent.end())
            break;
        current = it->second;
    }
    // Tính cả Start
    if (current == start)
    {
        pathLength++;
    }
    return true;
}