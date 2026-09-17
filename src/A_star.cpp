#include "A_star.h"
#include <cmath>
A_star::A_star() {}
int A_star::getSteps() const
{
    return steps;
}
int A_star::getPathLength() const
{
    return pathLength;
}
double A_star::getExecutionTime() const
{
    return executionTime;
}
int A_star::heuristic(Cell* a, Cell* b)
{
    return std::abs(a->getRow() - b->getRow())
         + std::abs(a->getCol() - b->getCol());
}
// Truy vết đường đi
void A_star::reconstructPath()
{
    pathLength = 0;
    Cell* current = goal;
    if (current == nullptr || start == nullptr) return;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState(CellState::PathAStar);
        }
        auto it = parent.find(current);
        if (it == parent.end()) break;
        current = it->second;
    }
    if (current == start)
    {
        pathLength++;
    }
}
// Khởi tạo A* animation
void A_star::startAlgorithm(Grid& grid)
{
    this->grid = &grid;
    while (!openSet.empty()) openSet.pop();
    gScore.clear();
    parent.clear();
    visited.clear();
    start = grid.getStart();
    goal = grid.getGoal();
    finished = false;
    found = false;
    steps = 0;
    pathLength = 0;
    executionTime = 0.0;
    if (start == nullptr || goal == nullptr)
    {
        finished = true;
        return;
    }
    animationStartTime = std::chrono::high_resolution_clock::now();
    gScore[start] = 0;
    int fScore = heuristic(start, goal);
    openSet.push({fScore, start});
    // A* = 8
    start->markVisited(8);
}
// Thực hiện một bước A*
bool A_star::step()
{
    if (finished)
        return false;
    if (grid == nullptr || start == nullptr || goal == nullptr)
    {
        finished = true;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>( endTime - animationStartTime).count();
        return false;
    }
    if (openSet.empty())
    {
        finished = true;
        found = false;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>( endTime - animationStartTime).count();
        return false;
    }
    auto [currentF, current] = openSet.top();
    openSet.pop();
    if (visited.find(current) != visited.end())
        return true;
    visited.insert(current);
    steps++;
    // Tìm thấy Goal
    if (current == goal)
    {
        found = true;
        reconstructPath();
        finished = true;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>( endTime - animationStartTime).count();
        return false;
    }
    std::vector<Cell*> neighbors =  grid->getNeighbors(current);
    for (Cell* neighbor : neighbors)
    {
        if (visited.find(neighbor) != visited.end())
            continue;
        int tentativeG = gScore[current] + 1;
        auto it = gScore.find(neighbor);
        if (it == gScore.end() || tentativeG < it->second)
        {
            gScore[neighbor] = tentativeG;
            parent[neighbor] = current;
            int fScore = tentativeG + heuristic(neighbor, goal);
            neighbor->markVisited(8);
            openSet.push({ fScore, neighbor});
        }
    }
    return true;
}

bool A_star::isFinished() const
{
    return finished;
}
bool A_star::isFound() const
{
    return found;
}
// Chạy toàn bộ A*, không animation
bool A_star::solve(Grid& grid)
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
        executionTime = std::chrono::duration<double, std::milli>( endTime - startTime).count();
        return false;
    }
    struct Compare
    {
        bool operator()(
            const std::pair<int, Cell*>& a,
            const std::pair<int, Cell*>& b
        ) const
        {
            return a.first > b.first;
        }
    };
    std::priority_queue<
        std::pair<int, Cell*>,
        std::vector<std::pair<int, Cell*>>,
        Compare> pq;
    std::unordered_map<Cell*, int> gScoreSolve;
    std::unordered_map<Cell*, Cell*> parentSolve;
    std::unordered_set<Cell*> visitedSolve;
    gScoreSolve[start] = 0;
    pq.push({heuristic(start, goal),start});
    // A* = 8
    start->markVisited(8);
    bool foundSolve = false;
    while (!pq.empty())
    {
        auto [currentF, current] = pq.top();
        pq.pop();
        if (visitedSolve.find(current) != visitedSolve.end())
        {
            continue;
        }
        visitedSolve.insert(current);
        steps++;
        if (current == goal)
        {
            foundSolve = true;
            break;
        }
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            if (visitedSolve.find(neighbor) != visitedSolve.end())
            {
                continue;
            }
            int tentativeG = gScoreSolve[current] + 1;
            auto it = gScoreSolve.find(neighbor);
            if (it == gScoreSolve.end() || tentativeG < it->second)
            {
                gScoreSolve[neighbor] = tentativeG;
                parentSolve[neighbor] = current;
                int fScore = tentativeG + heuristic(neighbor, goal);
                neighbor->markVisited(8);
                pq.push({fScore, neighbor});
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
    if (!foundSolve)
    {
        return false;
    }
    Cell* current = goal;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState(CellState::PathAStar);
        }
        auto it = parentSolve.find(current);
        if (it == parentSolve.end())
            break;
        current = it->second;
    }
    if (current == start)
    {
        pathLength++;
    }
    return true;
}