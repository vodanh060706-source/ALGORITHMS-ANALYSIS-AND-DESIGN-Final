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
    public:
        Cell(int row, int col, float size);
        void draw(sf::RenderWindow &Window);
        void setState(CellState state);
        CellState getState() const;
        bool contains(float x, float y) const;
};