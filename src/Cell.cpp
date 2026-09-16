#include "Cell.h"
Cell::Cell(int row, int col, float size)
    : row(row),
      col(col),
      size(size),
      state(CellState::Empty),
      visitedMask(0)
{
    shape.setSize(sf::Vector2f(size - 1.0f, size - 1.0f));

    shape.setPosition(
        col * size,
        row * size
    );

    shape.setFillColor(sf::Color::White);
}
int Cell::getRow() const{
    return row;
 };
int Cell::getCol() const{
    return col;
};
void Cell::draw(sf::RenderWindow& window)
{
    switch (state)
    {
        case CellState::Empty:
            // Màu nền ô trống (màu tối hoặc xám nhẹ để nổi bật trên nền cửa sổ)
            shape.setFillColor(sf::Color(50, 100, 50));
            break;

        case CellState::Wall:
            // Màu của tường chắn khi vẽ chuột trái
            shape.setFillColor(sf::Color(15, 15, 15));
            break;

        case CellState::Start:
            shape.setFillColor(sf::Color(0, 220, 80));
            break;

        case CellState::Goal:
            shape.setFillColor(sf::Color(240, 50, 50));
            break;

        case CellState::VisitedBFS:
            shape.setFillColor(sf::Color(255, 220, 0));
            break;

        case CellState::VisitedDFS:
            shape.setFillColor(sf::Color(50, 150, 255));
            break;

        case CellState::VisitedDijkstra:
            shape.setFillColor(sf::Color(180, 70, 255));
            break;

        case CellState::VisitedAStar:
            shape.setFillColor(sf::Color(255, 140, 0));
            break;

        case CellState::Overlap:
            shape.setFillColor(sf::Color(0, 220, 200));
            break;

        case CellState::PathBFS:
            // Đổi sang màu vàng sáng hoặc xanh để không bị trùng màu trắng
            shape.setFillColor(sf::Color(255, 255, 100));
            break;

        case CellState::PathDFS:
            shape.setFillColor(sf::Color(255, 120, 220));
            break;

        case CellState::PathDijkstra:
            shape.setFillColor(sf::Color(120, 255, 120));
            break;

        case CellState::PathAStar:
            shape.setFillColor(sf::Color(255, 100, 50));
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
void Cell::setOffset(float offsetX, float offsetY) {
    shape.setPosition(
        offsetX + col * size,
        offsetY + row * size
    );
}
void Cell::markVisited(int algorithmBit)
{
    // Start và Goal luôn giữ màu
    if (state == CellState::Start ||
        state == CellState::Goal)
    {
        // Vẫn ghi nhận thuật toán đã đi qua
        visitedMask |= algorithmBit;
        return;
    }

    // Ghi nhận thuật toán đã đi qua ô
    visitedMask |= algorithmBit;

    // Chỉ BFS
    if (visitedMask == 1)
    {
        state = CellState::VisitedBFS;
    }
    // Chỉ DFS
    else if (visitedMask == 2)
    {
        state = CellState::VisitedDFS;
    }
    // Chỉ Dijkstra
    else if (visitedMask == 4)
    {
        state = CellState::VisitedDijkstra;
    }
    // Chỉ A*
    else if (visitedMask == 8)
    {
        state = CellState::VisitedAStar;
    }
    // Có từ 2 thuật toán trở lên
    else
    {
        state = CellState::Overlap;
    }
}
bool Cell::hasBeenVisited(int algorithmBit) const
{
    return (visitedMask & algorithmBit) != 0;
}

int Cell::getVisitedMask() const
{
    return visitedMask;
}

void Cell::resetVisited()
{
    visitedMask = 0;
}