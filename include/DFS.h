#pragma once

#include "Algorithm.h"
#include "Grid.h"
#include <stack> 
#include <unordered_map> // Lưu cha mỗi ô 
#include <unordered_set> // Lưu vị trị đã thăm 
#include <vector>
class DFS : public Algorithm
{
private:
    int steps = 0;
public:
    DFS();
    bool solve(Grid& grid) override;
    int getSteps() const;
};