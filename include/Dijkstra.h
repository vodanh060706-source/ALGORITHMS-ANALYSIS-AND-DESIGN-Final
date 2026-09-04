#pragma once 

#include "Algorithm.h"
#include "Grid.h"
#include <queue> 
#include <unordered_map> // Lưu cha mỗi ô 
#include <unordered_set> // Lưu vị trị đã thăm 
#include <vector>
class Dijkstra : public Algorithm
{
public:
    Dijkstra();
    bool solve(Grid& grid) override;
};