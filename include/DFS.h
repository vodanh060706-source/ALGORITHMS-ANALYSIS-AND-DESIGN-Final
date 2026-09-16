#pragma once

#include "Algorithm.h"
#include "Grid.h"
#include <stack>
#include <unordered_map>   // Lưu cha mỗi ô
#include <unordered_set>   // Lưu vị trí đã thăm
#include <vector>
#include <chrono>
class DFS : public Algorithm
{
private:
    int steps = 0;
    int pathLength = 0;
    double executionTime = 0.0;

    Grid* grid = nullptr;

    std::stack<Cell*> stack;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;
    std::chrono::high_resolution_clock::time_point animationStartTime;
    Cell* start = nullptr;
    Cell* goal = nullptr;

    bool finished = false;
    bool found = false;

public:
    DFS();

    // Chạy toàn bộ DFS
    bool solve(Grid& grid) override;

    // Khởi tạo DFS để animation
    void startAlgorithm(Grid& grid);

    // Thực hiện 1 bước animation
    bool step();

    // Kiểm tra trạng thái
    bool isFinished() const;
    bool isFound() const;

    // Kết quả
    int getSteps() const;
    int getPathLength() const;
    double getExecutionTime() const;

private:
    // Truy vết đường đi
    void reconstructPath();
};