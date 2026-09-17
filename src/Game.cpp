#include "Game.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>
#include <sstream>
// CONSTRUCTOR
Game::Game()
    : rows(18),
      cols(24),
      cellSize(
          std::min(GRID_WIDTH / cols, GRID_HEIGHT / rows)),
      grid(rows, cols, cellSize),
      isRunning(true),
      window(sf::VideoMode(
                static_cast<unsigned int>(WINDOW_WIDTH),
                static_cast<unsigned int>(WINDOW_HEIGHT)),
                "Path Finding Game")
{
    window.setFramerateLimit(60);
    // Tải font chữ
    if (!font.loadFromFile("assets/font.ttf"))
    {
        std::cerr
            << "ERROR: Cannot load assets/font.ttf\n";
    }
    else
    {
        std::cout<< "Font loaded successfully\n";
    }
    centerGrid();
    // SIDE PANEL
    sidePanel.setSize(sf::Vector2f(UI_WIDTH,UI_HEIGHT));
    sidePanel.setPosition(GRID_WIDTH,0.f);
    sidePanel.setFillColor(sf::Color(30, 30, 30));
    // Bảng thông tin 
    infoPanel.setSize(sf::Vector2f(220.f,230.f));
    infoPanel.setPosition(GRID_WIDTH + 20.f,110.f);
    infoPanel.setFillColor(sf::Color(42, 42, 42));
    infoPanel.setOutlineColor(sf::Color(90, 90, 90));
    infoPanel.setOutlineThickness(1.f);
    // Tiêu đề 
    titleText.setFont(font);
    titleText.setString("PATH FINDING");
    titleText.setCharacterSize(28);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(GRID_WIDTH + 35.f,28.f);
    // Thuật toán 
    algorithmText.setFont(font);
    algorithmText.setString("Algorithm: Ready");
    algorithmText.setCharacterSize(17);
    algorithmText.setFillColor(sf::Color::White);
    algorithmText.setPosition(GRID_WIDTH + 35.f,135.f);
    // Số bước 
    stepsText.setFont(font);
    stepsText.setString("Steps: --");
    stepsText.setCharacterSize(17);
    stepsText.setFillColor(sf::Color::White);
    stepsText.setPosition(GRID_WIDTH + 35.f,175.f);
    // Đường đi 
    pathText.setFont(font);
    pathText.setString("Path: --");
    pathText.setCharacterSize(17);
    pathText.setFillColor(sf::Color::White);
    pathText.setPosition(GRID_WIDTH + 35.f,215.f);
    // Thời gian 
    timeText.setFont(font);
    timeText.setString(
        "Time: --"
    );
    timeText.setCharacterSize(17);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(GRID_WIDTH + 35.f,255.f);
    // Điều khiển 
    controlsText.setFont(font);
    controlsText.setString(
        "CONTROLS\n\n"
        "S    Select Start\n"
        "G    Select Goal\n"
        "ESC  Cancel\n\n"
        "B    BFS\n"
        "D    DFS\n"
        "K    Dijkstra\n"
        "F    GBFS\n"
        "A    A*\n\n"
        "R    Random Maze\n"
        "T    Random Maze Full\n"
        "M    Benrmark Table\n"
        "C    Clear"
    );
    controlsText.setCharacterSize(15);
    controlsText.setFillColor(
        sf::Color(220, 220, 220)
    );
    controlsText.setPosition(
        GRID_WIDTH + 30.f,
        390.f
    );
        // Phần Benrmark 
    compareText.setFont(font);
    compareText.setCharacterSize(18);
    compareText.setFillColor(sf::Color::White);
    compareText.setPosition(30.f, 670.f);
    compareText.setString("COMPARISON");
}
Game::~Game() {}
void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}
void Game::compareAlgorithms()
{
    // bfs
    grid.clearPath();
    bfs.solve(grid);
    int bfsSteps = bfs.getSteps();
    int bfsPath = bfs.getPathLength();
    double bfsTime = bfs.getExecutionTime();
    // dfs
    grid.clearPath();
    dfs.solve(grid);
    int dfsSteps = dfs.getSteps();
    int dfsPath = dfs.getPathLength();
    double dfsTime = dfs.getExecutionTime();
    // dijkstra
    grid.clearPath();
    dijkstra.solve(grid);
    int dijkstraSteps = dijkstra.getSteps();
    int dijkstraPath = dijkstra.getPathLength();
    double dijkstraTime = dijkstra.getExecutionTime();
    // a_star
    grid.clearPath();
    aStar.solve(grid);
    int aStarSteps = aStar.getSteps();
    int aStarPath = aStar.getPathLength();
    double aStarTime = aStar.getExecutionTime();
    // gbfs
    grid.clearPath();
    gbfs.solve(grid);
    int gbfsSteps = gbfs.getSteps();
    int gbfsPath = gbfs.getPathLength();
    double gbfsTime = gbfs.getExecutionTime();

    std::ostringstream result;
    result << "COMPARISON\n\n"
        << "BFS:          " << bfsSteps
        << "steps     |    " << bfsPath
        << "path      |    " << std::fixed
        << std::setprecision(3)
        << bfsTime << " ms\n"

        << "DFS:          " << dfsSteps
        << "steps     |    " << dfsPath
        << "path      |    "
        << dfsTime << "ms\n"

        << "Dijkstra:     " << dijkstraSteps
        << "steps     |   " << dijkstraPath
        << "path      |   "
        << dijkstraTime << " ms\n"

        << "A*:           " << aStarSteps
        << "steps      |   " << aStarPath
        << "path       |   "
        << aStarTime << " ms\n"

        << "GBFS:          " << gbfsSteps
        << "steps     |    " << gbfsPath
        << "path      |    "
        << gbfsTime << "ms\n";
    compareText.setString(result.str());
}
// UPDATE
void Game::update()
{
    // ANIMATION BFS / DFS
    if (isAnimating)
    {
        if (animationClock.getElapsedTime().asSeconds() >= animationDelay)
        {
            animationClock.restart();
            bool running = true;
            // BFS
            if (animationAlgorithm == AnimationAlgorithm::BFS)
            {
                running = bfs.step();
                stepsText.setString( "Steps: " + std::to_string(bfs.getSteps()));
                pathText.setString( "Path: " + std::to_string(bfs.getPathLength()));
                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << bfs.getExecutionTime();
                    timeText.setString( "Time: " + timeStream.str() + " ms");
                }
            }
            // DFS
            else if (animationAlgorithm == AnimationAlgorithm::DFS)
            {
                running = dfs.step();
                stepsText.setString( "Steps: " + std::to_string(dfs.getSteps()));
                pathText.setString( "Path: " + std::to_string(dfs.getPathLength()));
                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3) << dfs.getExecutionTime();
                    timeText.setString( "Time: " + timeStream.str() + " ms");
                }
            }
            // Dijkstra
            else if (animationAlgorithm == AnimationAlgorithm::Dijkstra)
            {
                running = dijkstra.step();
                stepsText.setString("Steps: " + std::to_string( dijkstra.getSteps()));
                pathText.setString( "Path: " +std::to_string( dijkstra.getPathLength()));
                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                     << dijkstra.getExecutionTime();
                    timeText.setString( "Time: " + timeStream.str() +" ms");
                }
            }
            // A_star 
            else if (animationAlgorithm == AnimationAlgorithm::A_star)
            {
                running = aStar.step();
                stepsText.setString("Steps: " + std::to_string(aStar.getSteps()));
                pathText.setString( "Path: " +std::to_string(aStar.getPathLength()));
                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                     << aStar.getExecutionTime();
                    timeText.setString( "Time: " + timeStream.str() +" ms");
                }
            }
            // GBFS
            if (animationAlgorithm == AnimationAlgorithm::GBFS)
            {
                running = gbfs.step();
                stepsText.setString( "Steps: " + std::to_string(gbfs.getSteps()));
                pathText.setString( "Path: " + std::to_string(gbfs.getPathLength()));
                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << gbfs.getExecutionTime();
                    timeText.setString( "Time: " + timeStream.str() + " ms");
                }
            }
        }
    }
    // CHUỘT TRÁI: VẼ WALL
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!selectingStart && !selectingGoal && !isAnimating)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            Cell* cell = grid.getCellAt(static_cast<float>(mousePosition.x),static_cast<float>(mousePosition.y));
            if (cell != nullptr)
            {
                if (cell->getState() != CellState::Start && cell->getState() != CellState::Goal)
                {
                    cell->setState(CellState::Wall);
                }
            }
        }
    }
    // CHUỘT PHẢI: XÓA CELL
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!isAnimating)
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
}
// CENTER GRID
void Game::centerGrid()
{
    float gridPixelWidth = cols * cellSize;
    float gridPixelHeight = rows * cellSize;
    float offsetX =
        (GRID_WIDTH - gridPixelWidth) / 2.f;
    float offsetY =
        (GRID_HEIGHT - gridPixelHeight) / 2.f;
    grid.setOffset(offsetX, offsetY);
}
void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // ĐÓNG CỬA SỔ
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        // BÀN PHÍM
        if (event.type == sf::Event::KeyPressed)
        {
            // CHỌN START
            if (event.key.code == sf::Keyboard::S)
            {
                if (isAnimating)
                    continue;
                selectingStart = true;
                selectingGoal = false;
                algorithmText.setString(
                    "Algorithm: Select Start"
                );
            }
            // CHỌN GOAL
            else if (event.key.code == sf::Keyboard::G)
            {
                if (isAnimating)
                    continue;
                selectingStart = false;
                selectingGoal = true;
                algorithmText.setString(
                    "Algorithm: Select Goal"
                );
            }
            // BFS
            else if (event.key.code == sf::Keyboard::B)
            {
                if (isAnimating) continue;
                selectingStart = false;
                selectingGoal = false;
                grid.clearPath();
                bfs.startAlgorithm(grid);
                animationAlgorithm = AnimationAlgorithm::BFS;
                isAnimating = true;
                animationClock.restart();
                algorithmText.setString("Algorithm: BFS");
                stepsText.setString("Steps: 0");
                pathText.setString("Path: 0");
                timeText.setString("Time: 0.000 ms");
            }
            // DFS
            else if (event.key.code == sf::Keyboard::D)
            {
                if (isAnimating) continue;
                selectingStart = false;
                selectingGoal = false;
                grid.clearPath();
                dfs.startAlgorithm(grid);
                animationAlgorithm = AnimationAlgorithm::DFS;
                isAnimating = true;
                animationClock.restart();
                algorithmText.setString("Algorithm: DFS");
                stepsText.setString("Steps: 0");
                pathText.setString("Path: 0");
                timeText.setString("Time: 0.000 ms");
            }
            // DIJKSTRA
            else if (event.key.code == sf::Keyboard::K)
            {
                if (isAnimating) continue;
                selectingStart = false;
                selectingGoal = false;
                grid.clearPath();
                dijkstra.startAlgorithm(grid);
                animationAlgorithm = AnimationAlgorithm::Dijkstra;
                isAnimating = true;
                animationClock.restart();
                algorithmText.setString("Algorithm: Dijkstra");
                stepsText.setString("Steps: 0");
                pathText.setString("Path: 0");
                timeText.setString("Time: 0.000 ms");
            }
            // A*
            else if (event.key.code == sf::Keyboard::A)
            {
                if (isAnimating) continue;
                selectingStart = false;
                selectingGoal = false;
                grid.clearPath();
                aStar.startAlgorithm(grid);
                animationAlgorithm = AnimationAlgorithm::A_star;
                isAnimating = true;
                animationClock.restart();
                algorithmText.setString("Algorithm: A*");
                stepsText.setString("Steps: 0");
                pathText.setString("Path: 0");
                timeText.setString("Time: 0.000 ms");
            }
            // GBFS
            else if (event.key.code == sf::Keyboard::F)
            {
                if (isAnimating) continue;
                selectingStart = false;
                selectingGoal = false;
                grid.clearPath();
                gbfs.startAlgorithm(grid);
                animationAlgorithm = AnimationAlgorithm::GBFS;
                isAnimating = true;
                animationClock.restart();
                algorithmText.setString("Algorithm: GBFS");
                stepsText.setString("Steps: 0");
                pathText.setString("Path: 0");
                timeText.setString("Time: 0.000 ms");
            }
            // RANDOM MAZE
            else if (event.key.code == sf::Keyboard::R)
            {
                // Dừng animation
                isAnimating = false;
                selectingStart = false;
                selectingGoal = false;
                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<int> rowDist(11, 25);
                std::uniform_int_distribution<int> colDist(15, 35);
                rows = rowDist(gen);
                cols = colDist(gen);
                // Đảm bảo số lẻ
                if (rows % 2 == 0) rows++;
                if (cols % 2 == 0) cols++;
                cellSize = std::min( GRID_WIDTH / cols, GRID_HEIGHT / rows);
                grid.resize(rows, cols, cellSize);
                centerGrid();
                mazeGenerator.generate(grid);
                algorithmText.setString("Algorithm: Random Maze");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
            }
            // Tạo toàn bộ mê cung ngẫu nhiên bao gồm cả start và goal 
            else if (event.key.code == sf::Keyboard::T)
            {
                if (isAnimating)
                    continue;
                selectingStart = false;
                selectingGoal = false;
                mazeGenerator.generate(grid);
                std::vector<Cell*> emptyCells;
                for (int r = 0; r < rows; r++)
                {
                    for (int c = 0; c < cols; c++)
                    {
                        Cell* cell = grid.getCell(r, c);
                        if (cell != nullptr &&
                            cell->getState() == CellState::Empty)
                        {
                            emptyCells.push_back(cell);
                        }
                    }
                }
                if (emptyCells.size()>=2)
                {
                    static std::random_device rd;
                    static std::mt19937 gen(rd());
                    std::shuffle(
                        emptyCells.begin(),
                        emptyCells.end(),
                        gen
                    );
                    grid.setStart(emptyCells[0]);
                    grid.setGoal(emptyCells[1]);
                }
                algorithmText.setString("Algorithm: Auto Maze");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
            }
            // Bảng so sánh 
            else if (event.key.code == sf::Keyboard::M)
            {
                if (isAnimating)
                    continue;
                selectingStart = false;
                selectingGoal = false;
                compareAlgorithms();
                algorithmText.setString("Algorithm: Compare");
            }
            // ESC
            else if (event.key.code == sf::Keyboard::Escape)
            {
                selectingStart = false;
                selectingGoal = false;
                if (!isAnimating)
                {
                    algorithmText.setString("Algorithm: Ready");
                }
            }
            // C - CLEAR PATH
            else if (event.key.code == sf::Keyboard::C)
            {
                // Dừng animation
                isAnimating = false;
                selectingStart = false;
                selectingGoal = false;
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
            // Không cho thay đổi Grid khi animation
            if (isAnimating)
                continue;
            Cell* cell = grid.getCellAt(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );
            if (cell != nullptr)
            {
                // CHUỘT TRÁI
                if (event.mouseButton.button ==
                    sf::Mouse::Left)
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
                        algorithmText.setString("Algorithm: Ready");
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
                // CHUỘT PHẢI 
                else if (
                    event.mouseButton.button ==
                    sf::Mouse::Right)
                {
                    if (cell->getState() !=
                            CellState::Start &&
                        cell->getState() !=
                            CellState::Goal)
                    {
                        cell->setState(
                            CellState::Empty
                        );
                    }
                }
            }
        }
    }
}
// RENDER
void Game::render()
{
    window.clear(sf::Color(20, 20, 20));
    // Grid
    grid.draw(window);
    // Panel
    window.draw(sidePanel);
    window.draw(infoPanel);
    // Text
    window.draw(titleText);
    window.draw(algorithmText);
    window.draw(stepsText);
    window.draw(pathText);
    window.draw(timeText);
    window.draw(controlsText);
    window.draw(compareText);
    window.display();
}