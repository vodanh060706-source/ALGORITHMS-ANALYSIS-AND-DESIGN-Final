#include "GBFS.h"
#include <chrono>
GBFS::GBFS() {}

int GBFS::getPathLength() const
{
    return pathLength;
}

double GBFS::getExecutionTime() const
{
    return executionTime;
}

void GBFS::startAlgorithm(Grid& grid)
{
    this->grid = &grid;
    while (!openSet.empty())
        openSet.pop();
    visited.clear();
    parent.clear();

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

    openSet.push({
        heuristic(start, goal),
        start
    });

    // GBFS = 16
    start->markVisited(16);
}

bool GBFS::step()
{
    if (finished) return false;

    if (grid == nullptr || start == nullptr || goal == nullptr)
    {
        finished = true;
        return false;
    }
    if (openSet.empty())
    {
        finished = true;
        found = false;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(endTime - animationStartTime).count();
        return false;
    }

    auto [currentH, current] = openSet.top();
    openSet.pop();
    if (visited.find(current) != visited.end()) return true;
    visited.insert(current);
    steps++;
    // Tìm thấy Goal
    if (current == goal)
    {
        found = true;
        reconstructPath();
        finished = true;
        auto endTime = std::chrono::high_resolution_clock::now();
        executionTime = std::chrono::duration<double, std::milli>(endTime - animationStartTime).count();
        return false;
    }

    std::vector<Cell*> neighbors = grid->getNeighbors(current);
    for (Cell* neighbor : neighbors)
    {
        if (visited.find(neighbor) != visited.end()) continue;
        if (parent.find(neighbor) == parent.end())
        {
            parent[neighbor] = current;
            int h = heuristic(neighbor, goal);
            neighbor->markVisited(16);
            openSet.push({ h, neighbor});
        }
    }
    return true;
}
bool GBFS::isFinished() const
{
    return finished;
}

bool GBFS::isFound() const
{
    return found;
}

int GBFS::getSteps() const
{
    return steps;
}
int GBFS::heuristic(Cell* a, Cell* b)
{
    return std::abs(a->getRow() - b->getRow()) + std::abs(a->getCol() - b->getCol());
}
// Truy vết đường đi
void GBFS::reconstructPath()
{
    pathLength = 0;
    Cell* current = goal;
    if (current == nullptr || start == nullptr) return;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState( CellState::PathGBFS
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
// Chạy toàn bộ, không animation
bool GBFS::solve(Grid& grid)
{
    steps = 0;
    pathLength = 0;
    executionTime = 0.0;
    auto startTime =
        std::chrono::high_resolution_clock::now();

    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();

    if (start == nullptr || goal == nullptr)
    {
        auto endTime =
            std::chrono::high_resolution_clock::now();

        executionTime =
            std::chrono::duration<double, std::milli>(
                endTime - startTime
            ).count();

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
        Compare
    > pq;
    std::unordered_set<Cell*> visitedSolve;
    std::unordered_map<Cell*, Cell*> parentSolve;
    pq.push({heuristic(start, goal),start});
    start->markVisited(16);
    bool foundSolve = false;
    while (!pq.empty())
    {
        auto [currentH, current] =
            pq.top();

        pq.pop();

        if (visitedSolve.find(current) !=
            visitedSolve.end())
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
        std::vector<Cell*> neighbors = grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            if (visitedSolve.find(neighbor) != visitedSolve.end())
            {
                continue;
            }
            if (parentSolve.find(neighbor) == parentSolve.end())
            {
                parentSolve[neighbor] = current;
                int h = heuristic(neighbor, goal);
                neighbor->markVisited(16);
                pq.push({ h, neighbor });
            }
        }
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    executionTime = std::chrono::duration<double, std::milli>( endTime - startTime).count();
    if (!foundSolve)
        return false;
    Cell* current = goal;
    while (current != start)
    {
        pathLength++;
        if (current != goal)
        {
            current->setState(CellState::PathGBFS);
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