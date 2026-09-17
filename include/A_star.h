#pragma once
#include "Algorithm.h"
#include "Grid.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
class A_star : public Algorithm
{
private:
    struct NodeCompare
    {
        bool operator()(
            const std::pair<int, Cell*>& a,
            const std::pair<int, Cell*>& b
        ) const
        {
            return a.first > b.first;
        }
    };
    int steps = 0;
    int pathLength = 0;
    double executionTime = 0.0;
    Grid* grid = nullptr;
    std::priority_queue<
        std::pair<int, Cell*>,
        std::vector<std::pair<int, Cell*>>,
        NodeCompare> openSet;
    std::unordered_map<Cell*, int> gScore;
    std::unordered_map<Cell*, Cell*> parent;
    std::unordered_set<Cell*> visited;
    Cell* start = nullptr;
    Cell* goal = nullptr;
    bool finished = false;
    bool found = false;
    std::chrono::high_resolution_clock::time_point animationStartTime;
public:
    A_star();
    bool solve(Grid& grid) override;
    void startAlgorithm(Grid& grid);
    bool step();
    bool isFinished() const;
    bool isFound() const;
    int getSteps() const;
    int getPathLength() const;
    double getExecutionTime() const;
private:
    int heuristic(Cell* a, Cell* b);
    void reconstructPath();
};