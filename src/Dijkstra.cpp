#include "Dijkstra.h"

Dijkstra::Dijkstra() {}

int Dijkstra::getSteps() const {
    return steps;
}
int Dijkstra::getPathLength() const {
    return pathLength;
}
double Dijkstra::getExecutionTime() const {
    return executionTime;
}
// Truy vết đường đi
void Dijkstra::reconstructPath() {
    pathLength = 0;
    Cell* current = goal;
    if (current == nullptr || start == nullptr)
        return;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState(
                CellState::PathDijkstra
            );
        }
        auto it = parent.find(current);
        if (it == parent.end())
            break;
        current = it->second;
    }
    if (current == start)
    {
        pathLength++;
    }
}
// Khởi tạo Dijkstra animation
void Dijkstra::startAlgorithm(Grid& grid)
{
    this->grid = &grid;
    while (!pq.empty())
     pq.pop();
    distance.clear();
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
    animationStartTime =
        std::chrono::high_resolution_clock::now();
    distance[start] = 0;
    pq.push({0, start});
    // Dijkstra = 4
    start->markVisited(4);
}
// Thực hiện một bước
bool Dijkstra::step()
{
    if (finished) return false;
    if (grid == nullptr ||
        start == nullptr ||
        goal == nullptr)
        {
            finished = true;
            return false;
        }
    if (pq.empty())
    {
        finished = true;
        found = false;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>( endTime - animationStartTime).count();
        return false;
    }
    auto [currentDistance, current] =
        pq.top();
    pq.pop();
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
        int newDistance =  currentDistance + 1;
        auto it = distance.find(neighbor);
        if (it == distance.end() ||  newDistance < it->second)
        {
            distance[neighbor] = newDistance;
            parent[neighbor] = current;
            neighbor->markVisited(4);
            pq.push({ newDistance, neighbor});
        }
    }
    return true;
}
bool Dijkstra::solve(Grid& grid)
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
    struct Compare
    {
        bool operator()(
            const std::pair<int, Cell*>& a,
            const std::pair<int, Cell*>& b) const
            {
                return a.first > b.first;
            }
    };
    std::priority_queue<
        std::pair<int, Cell*>,
        std::vector<std::pair<int, Cell*>>,
        Compare
    > pqSolve;
    std::unordered_map<Cell*, int> distanceSolve;
    std::unordered_map<Cell*, Cell*> parentSolve;
    std::unordered_set<Cell*> visitedSolve;
    distanceSolve[start] = 0;
    pqSolve.push({0, start});
    // Dijkstra = 4
    start->markVisited(4);
    bool foundSolve = false;
    while (!pqSolve.empty())
    {
        auto [currentDistance, current] =
            pqSolve.top();
        pqSolve.pop();
        if (visitedSolve.find(current) !=
            visitedSolve.end())
        {
            continue;
        }
        visitedSolve.insert(current);
        steps++;
        // Tìm thấy Goal
        if (current == goal)
        {
            foundSolve = true;
            break;
        }
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            int newDistance = currentDistance + 1;
            auto it = distanceSolve.find(neighbor);
            if (it == distanceSolve.end() ||
                newDistance < it->second)
            {
                distanceSolve[neighbor] = newDistance;
                parentSolve[neighbor] = current;
                if (visitedSolve.find(neighbor) == visitedSolve.end())
                {
                    neighbor->markVisited(4);
                }
                pqSolve.push({ newDistance, neighbor});
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>( endTime - startTime).count();
    // Không tìm thấy Goal
    if (!foundSolve)
    {
        return false;
    }
    // Truy vết đường đi
    Cell* current = goal;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState(CellState::PathDijkstra);
        }
        auto it = parentSolve.find(current);
        if (it == parentSolve.end())
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
bool Dijkstra::isFinished() const
{
    return finished;
}

bool Dijkstra::isFound() const
{
    return found;
}