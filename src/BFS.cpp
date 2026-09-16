#include "BFS.h"
#include <chrono>

namespace
{
    std::chrono::high_resolution_clock::time_point animationStartTime;
}

BFS::BFS() {}

int BFS::getPathLength() const
{
    return pathLength;
}

double BFS::getExecutionTime() const
{
    return executionTime;
}

void BFS::startAlgorithm(Grid& grid)
{
    animationStartTime =
        std::chrono::high_resolution_clock::now();

    this->grid = &grid;

    queue = std::queue<Cell*>();
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

    queue.push(start);
    visited.insert(start);

    start->markVisited(1);
}

bool BFS::step()
{
    if (finished)
        return false;

    if (queue.empty())
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

    Cell* current = queue.front();
    queue.pop();

    steps++;

    if (current == goal)
    {
        found = true;
        finished = true;

        reconstructPath();

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

            neighbor->markVisited(1);

            parent[neighbor] = current;

            queue.push(neighbor);
        }
    }

    return true;
}

void BFS::reconstructPath()
{
    Cell* current = goal;

    pathLength = 0;

    while (current != start)
    {
        pathLength++;

        if (current != goal)
        {
            current->setState(
                CellState::PathBFS
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

bool BFS::isFinished() const
{
    return finished;
}

bool BFS::isFound() const
{
    return found;
}

int BFS::getSteps() const
{
    return steps;
}

bool BFS::solve(Grid& grid)
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

    std::queue<Cell*> queue;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;

    queue.push(start);
    visited.insert(start);

    start->markVisited(1);

    while (!queue.empty())
    {
        Cell* current = queue.front();
        queue.pop();

        steps++;

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

                neighbor->markVisited(1);

                parent[neighbor] = current;

                queue.push(neighbor);
            }
        }
    }

    if (visited.find(goal) == visited.end())
    {
        auto endTime =
            std::chrono::high_resolution_clock::now();

        executionTime =
            std::chrono::duration<double, std::milli>(
                endTime - startTime
            ).count();

        return false;
    }

    Cell* current = goal;

    while (current != start)
    {
        pathLength++;

        if (current != goal)
        {
            current->setState(
                CellState::PathBFS
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

    auto endTime =
        std::chrono::high_resolution_clock::now();

    executionTime =
        std::chrono::duration<double, std::milli>(
            endTime - startTime
        ).count();

    return true;
}