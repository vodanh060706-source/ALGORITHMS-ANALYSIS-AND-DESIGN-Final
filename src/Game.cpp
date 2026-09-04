#include "Game.h" 

Game::Game()
    : rows(18),
      cols(24),
      cellSize(std::min(
          WINDOW_WIDTH / cols,
          WINDOW_HEIGHT / rows
      )),
      grid(rows, cols, cellSize),
      isRunning(true),
      window(
          sf::VideoMode(960, 720),
          "Path Finding Game"
      )
{}
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
        // Nhấn phím chọn mục tiêu 
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::S)
            {
                selectingStart = true;
                selectingGoal = false;
            }
            else if (event.key.code == sf::Keyboard::G)
            {
                selectingStart = false;
                selectingGoal = true;
            }
            else if (event.key.code == sf::Keyboard::B)
            {
                bfs.solve(grid);
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                dfs.solve(grid);
            }
            else if (event.key.code == sf::Keyboard::K)
            {
                dijkstra.solve(grid);
            }
            else if (event.key.code == sf::Keyboard::A)
            {
                aStar.solve(grid);
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                rows = 25;
                cols = 35;

                cellSize = std::min(
                    WINDOW_WIDTH / cols,
                    WINDOW_HEIGHT / rows
                );

                grid.resize(rows, cols, cellSize);
            }
            else
            {
                selectingStart = false;
                selectingGoal = false;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            Cell* cell = grid.getCellAt(
                event.mouseButton.x,
                event.mouseButton.y
            );

            if (cell != nullptr)
            {
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cell->setState(CellState::Empty);
                }

                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (selectingStart)
                    {
                        grid.setStart(cell);
                    }
                    else if (selectingGoal)
                    {
                        grid.setGoal(cell);
                    }
                    else
                    {
                        cell->setState(CellState::Wall);
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
    grid.draw(window);
    // - Wall
    // - Start
    // - Goal
    // - Path
    // - UI
    window.display();
}