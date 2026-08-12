#include "Game.h" 

Game::Game()
    : window(sf::VideoMode(1000, 800), "Path Finding Game"),
    isRunning(true), grid(20,25,40.f) {}

Game::~Game()
{
}
void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        // Nhấn phím S
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::S){
                selectingStart = true;
                selectingGoal = false;
            }
            if(event.key.code == sf::Keyboard::G){
                selectingStart = false;
                selectingGoal = true;
            }
        }
        // Click chuột
        if (event.type == sf::Event::MouseButtonPressed)
        {
            Cell* cell = grid.getCellAt(
                event.mouseButton.x, 
                event.mouseButton.y
            );
            if (cell != nullptr)
            {
                // Chuột phải → xóa Wall
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cell->setState(CellState::Empty);
                }
                // Chuột trái → tạo Wall
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if(selectingStart) {
                        grid.setStart(cell);
                        selectingStart = false;
                    }
                    else if(selectingGoal) {
                        grid.setGoal(cell);
                        selectingGoal = false;
                    }
                    else {
                        cell -> setState(CellState::Wall);
                    }
                }
            }
        }
        // Kéo chuột — vẽ/xóa wall khi giữ chuột
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
            sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            Cell* cell = grid.getCellAt(
                mousePosition.x,
                mousePosition.y
            );
            if (cell != nullptr)
            { 
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    // Khi đang chọn start/goal thì kéo không tạo wall
                    if (!selectingStart && !selectingGoal)
                        cell->setState(CellState::Wall);
                }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                {
                    cell->setState(CellState::Empty);
                }
            }
        }
    }
}
void Game::update()
{
    // Chưa có gì để cập nhật
}

void Game::render()
{
    window.clear(sf::Color::Black);

    // Sau này sẽ vẽ:
    grid.draw(window);
    // - Wall
    // - Start
    // - Goal
    // - Path
    // - UI

    window.display();
}