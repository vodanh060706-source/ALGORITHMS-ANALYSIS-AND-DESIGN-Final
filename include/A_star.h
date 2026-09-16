#pragma once 

#include "Algorithm.h"
#include "Grid.h"
#include <queue> 
#include <cmath>
#include <unordered_map> // Lưu cha mỗi ô 
#include <unordered_set> // Lưu vị trị đã thăm 
#include <vector>
class A_star : public Algorithm
{
private:
    int steps = 0;
public:
    A_star();
    bool solve(Grid& grid) override;
    int getSteps() const;
};