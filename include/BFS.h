#pragma once

#include "Algorithm.h"
#include "Grid.h"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
class BFS : public Algorithm
{
public:
    BFS();
    bool solve(Grid& grid) override;
};