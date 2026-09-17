#pragma once 

#include <SFML/Graphics.hpp>
#include "CellState.h"
class Cell {
    private:
        int row;
        int col;
        float size;
        sf::RectangleShape shape;
        CellState state;
        int visitedMask = 0;
    public:
        Cell(int row, int col, float size);
        void draw(sf::RenderWindow &Window);
        void setState(CellState state);
        CellState getState() const;
        int getRow() const;
        int getCol() const;
        void markVisited(int algorithmBit);
        bool hasBeenVisited(int algorithmBit) const;
        int getVisitedMask() const;
        void resetVisited();
        bool contains(float x, float y) const;
        void setOffset(float offsetX, float offsetY);
};