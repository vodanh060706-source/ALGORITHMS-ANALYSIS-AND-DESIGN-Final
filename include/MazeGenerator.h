#pragma once 
#include "Grid.h"
class MazeGenerator {
    public: 
    void generate(Grid& grid);
    void carve(Grid& grid, int row, int col);
};