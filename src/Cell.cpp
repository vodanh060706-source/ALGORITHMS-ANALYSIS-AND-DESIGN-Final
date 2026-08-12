#include "Cell.h"

Cell::Cell(int row, int col, float size)
    : row(row),
      col(col),
      size(size),
      state(CellState::Empty)
{
    shape.setSize(sf::Vector2f(size - 1.0f, size - 1.0f));
    shape.setPosition(
        col * size,
        row * size
    );

    shape.setFillColor(sf::Color::White);
}

void Cell::draw(sf::RenderWindow& window)
{
    switch (state)
    {
    case CellState::Empty:
        shape.setFillColor(sf::Color::White);
        break;
    case CellState::Wall:
        shape.setFillColor(sf::Color::Black);
        break;
    case CellState::Start:
        shape.setFillColor(sf::Color::Green);
        break;
    case CellState::Goal:
        shape.setFillColor(sf::Color::Red);
        break;
    case CellState::Visited:
        shape.setFillColor(sf::Color::Blue);
        break;
    case CellState::Path:
        shape.setFillColor(sf::Color::Yellow);
        break;
    }
    window.draw(shape);
}
CellState Cell::getState() const {
    return state;
}
void Cell::setState(CellState state) {
        this -> state = state;
}
bool Cell::contains(float x, float y) const {
    return shape.getGlobalBounds().contains(x,y);
}