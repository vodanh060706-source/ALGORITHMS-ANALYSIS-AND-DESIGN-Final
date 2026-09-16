#include "Game.h"
#include <algorithm>
#include <random>
#include <iostream>
// Khởi động game 
Game::~Game() { }
void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}
void Game::update()
{
    // Chuột trái: vẽ Wall
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
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
                if (cell->getState() != CellState::Start &&
                    cell->getState() != CellState::Goal)
                {
                    cell->setState(CellState::Wall);
                }
            }
        }
    }

    // Chuột phải: xóa
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
            if (cell->getState() != CellState::Start &&
                cell->getState() != CellState::Goal)
            {
                cell->setState(CellState::Empty);
            }
        }
    }
}
void Game::centerGrid()
{
    float gridPixelWidth = cols * cellSize;
    float gridPixelHeight = rows * cellSize;
    float offsetX = (GRID_WIDTH - gridPixelWidth) / 2.f;
    float offsetY = (GRID_HEIGHT - gridPixelHeight) / 2.f;
    grid.setOffset(offsetX, offsetY);
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
        // BÀN PHÍM
        if (event.type == sf::Event::KeyPressed)
        {
            // Chọn Start
            if (event.key.code == sf::Keyboard::S)
            {
                selectingStart = true;
                selectingGoal = false;
                algorithmText.setString("Algorithm: Ready");
            }
            // Chọn Goal
            else if (event.key.code == sf::Keyboard::G)
            {
                selectingStart = false;
                selectingGoal = true;
                algorithmText.setString("Algorithm: Ready");
            }
            // BFS
            else if (event.key.code == sf::Keyboard::B)
            {
                selectingStart = false;
                selectingGoal = false;
                bfs.solve(grid);
                stepsText.setString("Steps: " + std::to_string(bfs.getSteps()));
                algorithmText.setString("Algorithm: BFS");
            }
            // DFS
            else if (event.key.code == sf::Keyboard::D)
            {
                selectingStart = false;
                selectingGoal = false;
                dfs.solve(grid);
                stepsText.setString("Steps: " + std::to_string(dfs.getSteps()));
                algorithmText.setString("Algorithm: DFS");
            }
            // Dijkstra
            else if (event.key.code == sf::Keyboard::K)
            {
                selectingStart = false;
                selectingGoal = false;
                dijkstra.solve(grid);
                stepsText.setString("Steps: " + std::to_string(dijkstra.getSteps()));
                algorithmText.setString("Algorithm: Dijkstra");
            }
            // A*
            else if (event.key.code == sf::Keyboard::A)
            {
                selectingStart = false;
                selectingGoal = false;
                aStar.solve(grid);
                stepsText.setString("Steps: " + std::to_string(aStar.getSteps()));
                algorithmText.setString("Algorithm: A*");
            }
            // Random Maze
            else if (event.key.code == sf::Keyboard::R)
            {
                selectingStart = false;
                selectingGoal = false;
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<int> rowDist(11, 25);
                std::uniform_int_distribution<int> colDist(15, 35);
                rows = rowDist(gen);
                cols = colDist(gen);
                // Đảm bảo số lẻ
                if (rows % 2 == 0)
                    rows++;
                if (cols % 2 == 0)
                    cols++;
                cellSize = std::min(
                    GRID_WIDTH / cols,
                    GRID_HEIGHT / rows
                );
                grid.resize(
                    rows,
                    cols,
                    cellSize
                );
                centerGrid();
                mazeGenerator.generate(grid);
                algorithmText.setString("Algorithm: Random Maze");
            }
            // ESC
            else if (event.key.code == sf::Keyboard::Escape)
            {
                selectingStart = false;
                selectingGoal = false;
                algorithmText.setString("Algorithm: Ready");
            }
            // C 
            else if (event.key.code == sf::Keyboard::C)
            {
                grid.clearPath();
                algorithmText.setString("Algorithm: Ready");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
            }
        }
        // CHUỘT CLICK
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
                    // Chọn Start
                    if (selectingStart)
                    {
                        grid.setStart(cell);
                        selectingStart = false;
                        algorithmText.setString(
                            "Algorithm: Ready"
                        );
                    }
                    // Chọn Goal
                    else if (selectingGoal)
                    {
                        grid.setGoal(cell);
                        selectingGoal = false;
                        algorithmText.setString(
                            "Algorithm: Ready"
                        );
                    }
                    // Tạo Wall
                    else
                    {
                        if (cell->getState() != CellState::Start &&
                            cell->getState() != CellState::Goal)
                        {
                            cell->setState(CellState::Wall);
                        }
                    }
                }
                // Chuột phải
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
                    if (cell->getState() != CellState::Start &&
                        cell->getState() != CellState::Goal)
                    {
                        cell->setState(CellState::Empty);
                    }
                }
            }
        }
    }
}

void Game::render()
{
    window.clear(
        sf::Color(20, 20, 20)
    );
    // Thông tin cửa sổ 
    grid.draw(window);
    window.draw(sidePanel);
    window.draw(infoPanel);
    // tiêu đề cửa sổ 
    window.draw(titleText);
    window.draw(algorithmText);
    window.draw(stepsText);
    window.draw(pathText);
    window.draw(timeText);
    window.draw(controlsText);
    window.display();
}
Game::Game()
    : rows(18),
      cols(24),
      cellSize(std::min(
          GRID_WIDTH / cols,
          GRID_HEIGHT / rows
      )),
      grid(rows, cols, cellSize),
      isRunning(true),
      window(
          sf::VideoMode(
              static_cast<unsigned int>(WINDOW_WIDTH),
              static_cast<unsigned int>(WINDOW_HEIGHT)
          ),
          "Path Finding Game"
      )
{
    window.setFramerateLimit(60);
    // TẢI FONT 
    if (!font.loadFromFile("assets/font.ttf"))
    {
        std::cerr << "ERROR: Cannot load assets/font.ttf\n";
    }
    else
    {
        std::cout << "Font loaded successfully\n";
    }
    centerGrid();
    // THÔNG SỐ WINDOW 
    sidePanel.setSize(sf::Vector2f(UI_WIDTH, UI_HEIGHT));
    sidePanel.setPosition(GRID_WIDTH,0.f);
    sidePanel.setFillColor(sf::Color(30, 30, 30));
    // THÔNG TIN WINDOW 
    infoPanel.setSize(sf::Vector2f(220.f, 230.f));
    infoPanel.setPosition(GRID_WIDTH + 20.f,110.f);
    infoPanel.setFillColor(sf::Color(42, 42, 42));
    infoPanel.setOutlineColor(sf::Color(90, 90, 90));
    infoPanel.setOutlineThickness(1.f);
    // TIÊU ĐỀ 
    titleText.setFont(font);
    titleText.setString("PATH FINDING");
    titleText.setCharacterSize(28);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(GRID_WIDTH + 35.f,28.f);
    // THUẬT TOÁN 
    algorithmText.setFont(font);
    algorithmText.setString(
        "Algorithm: Ready"
    );
    algorithmText.setCharacterSize(17);
    algorithmText.setFillColor(sf::Color::White);
    algorithmText.setPosition(GRID_WIDTH + 35.f,135.f);
    // ĐƯỜNG ĐI 
    stepsText.setFont(font);
    stepsText.setString(
        "Steps: --"
    );
    stepsText.setCharacterSize(17);
    stepsText.setFillColor(
        sf::Color::White
    );
    stepsText.setPosition(
        GRID_WIDTH + 35.f,
        175.f
    );
    // ĐƯỜNG ĐI 
    pathText.setFont(font);
    pathText.setString(
        "Path: --"
    );
    pathText.setCharacterSize(17);

    pathText.setFillColor(
        sf::Color::White
    );
    pathText.setPosition(
        GRID_WIDTH + 35.f,
        215.f
    );
    // THỜI GIAN 
    timeText.setFont(font);
    timeText.setString("Time: --");
    timeText.setCharacterSize(17);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(GRID_WIDTH + 35.f,255.f);
    // ĐIỀU KHIỂN 
    controlsText.setFont(font);
    controlsText.setString(
        "CONTROLS\n\n"
        "S    Select Start\n"
        "G    Select Goal\n"
        "ESC  Cancel\n\n"
        "B    BFS\n"
        "D    DFS\n"
        "K    Dijkstra\n"
        "A    A*\n\n"
        "R    Random Maze"
    );
    controlsText.setCharacterSize(15);
    controlsText.setFillColor(sf::Color(220, 220, 220));
    controlsText.setPosition(GRID_WIDTH + 30.f,390.f);
}