#pragma once

#include "Algorithm.h"
#include "Grid.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <chrono>
class BFS : public Algorithm
{
private:
    int steps = 0;
    int pathLength = 0;
    double executionTime = 0.0;
    // Dùng cho animation
    Grid* grid = nullptr;
    std::queue<Cell*> queue;
    std::unordered_set<Cell*> visited;
    std::unordered_map<Cell*, Cell*> parent;

    Cell* start = nullptr;
    Cell* goal = nullptr;

    bool finished = false;
    bool found = false;
public:
    BFS();
    // Chạy toàn bộ BFS như trước
    bool solve(Grid& grid) override;
    // Khởi tạo BFS để animation
    void startAlgorithm(Grid& grid);
    // Thực hiện 1 bước
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