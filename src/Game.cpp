#include "Game.h"
#include <algorithm>

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
{
}

Game::~Game(){}

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
        // Đóng cửa sổ
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Xử lý bàn phím
        if (event.type == sf::Event::KeyPressed)
        {
            // Chọn Start
            if (event.key.code == sf::Keyboard::S)
            {
                selectingStart = true;
                selectingGoal = false;
            }

            // Chọn Goal
            else if (event.key.code == sf::Keyboard::G)
            {
                selectingStart = false;
                selectingGoal = true;
            }
            // BFS
            else if (event.key.code == sf::Keyboard::B)
            {
                selectingStart = false;
                selectingGoal = false;

                bfs.solve(grid);
            }
            // DFS
            else if (event.key.code == sf::Keyboard::D)
            {
                selectingStart = false;
                selectingGoal = false;

                dfs.solve(grid);
            }
            // Dijkstra
            else if (event.key.code == sf::Keyboard::K)
            {
                selectingStart = false;
                selectingGoal = false;

                dijkstra.solve(grid);
            }

            // A*
            else if (event.key.code == sf::Keyboard::A)
            {
                selectingStart = false;
                selectingGoal = false;

                aStar.solve(grid);
            }

            // Random Maze
            else if (event.key.code == sf::Keyboard::R)
            {
                selectingStart = false;
                selectingGoal = false;

                // Kích thước Maze
                rows = 25;
                cols = 35;

                // Tính kích thước ô
                cellSize = std::min(
                    WINDOW_WIDTH / cols,
                    WINDOW_HEIGHT / rows
                );

                // Tạo Grid mới
                grid.resize(rows, cols, cellSize);

                // Tạo Maze ngẫu nhiên
                mazeGenarator.generate(grid);
            }

            // Phím ESC: thoát chế độ chọn Start / Goal
            else if (event.key.code == sf::Keyboard::Escape)
            {
                selectingStart = false;
                selectingGoal = false;
            }
        }

        // Click chuột
        if (event.type == sf::Event::MouseButtonPressed)
        {
            Cell* cell = grid.getCellAt(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );

            if (cell != nullptr)
            {
                // Chuột trái
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    // Đang chọn Start
                    if (selectingStart)
                    {
                        grid.setStart(cell);
                        selectingStart = false;
                    }

                    // Đang chọn Goal
                    else if (selectingGoal)
                    {
                        grid.setGoal(cell);
                        selectingGoal = false;
                    }

                    // Bình thường: tạo Wall
                    else
                    {
                        cell->setState(CellState::Wall);
                    }
                }

                // Chuột phải: xóa Wall
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    cell->setState(CellState::Empty);
                }
            }
        }
    }
}

void Game::update()
{
    // Kéo chuột để vẽ / xóa Wall

    // Chuột trái
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        // Không kéo Wall khi đang chọn Start / Goal
        if (!selectingStart && !selectingGoal)
        {
            sf::Vector2i mousePosition =
                sf::Mouse::getPosition(window);

            Cell* cell = grid.getCellAt(
                static_cast<float>(mousePosition.x),
                static_cast<float>(mousePosition.y)
            );

            if (cell != nullptr)
            {
                // Không ghi đè Start / Goal
                if (cell->getState() != CellState::Start &&
                    cell->getState() != CellState::Goal)
                {
                    cell->setState(CellState::Wall);
                }
            }
        }
    }

    // Chuột phải
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        sf::Vector2i mousePosition =
            sf::Mouse::getPosition(window);

        Cell* cell = grid.getCellAt(
            static_cast<float>(mousePosition.x),
            static_cast<float>(mousePosition.y)
        );

        if (cell != nullptr)
        {
            // Không xóa Start / Goal
            if (cell->getState() != CellState::Start &&
                cell->getState() != CellState::Goal)
            {
                cell->setState(CellState::Empty);
            }
        }
    }
}

void Game::render()
{
    window.clear(sf::Color::Black);

    grid.draw(window);

    window.display();
}