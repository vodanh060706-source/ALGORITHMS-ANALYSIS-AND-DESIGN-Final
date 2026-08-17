#pragma once 
#include "Grid.h"
using namespace std;
class Algorithm {
    public:
        virtual ~Algorithm() = default;

        virtual bool solve(Grid& grih) = 0;
    
};