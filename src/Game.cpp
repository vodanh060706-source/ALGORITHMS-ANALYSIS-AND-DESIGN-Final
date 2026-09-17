#include "Game.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

Game::Game()
    : rows(18),
      cols(24),
      cellSize(std::min(GRID_WIDTH / cols, GRID_HEIGHT / rows)),
      grid(rows, cols, cellSize),
      isRunning(true),
      window(sf::VideoMode(
          static_cast<unsigned int>(WINDOW_WIDTH),
          static_cast<unsigned int>(WINDOW_HEIGHT)),
          "Path Finding Game")
{
    window.setFramerateLimit(60);

    if (!font.loadFromFile("assets/font.ttf"))
    {
        std::cerr << "ERROR: Cannot load assets/font.ttf\n";
    }
    else
    {
        std::cout << "Font loaded successfully\n";
    }

    setupButton(bfsButton, bfsButtonText, "BFS", 720.f, 360.f, 95.f, 30.f);
    setupButton(dfsButton, dfsButtonText, "DFS", 825.f, 360.f, 95.f, 30.f);
    setupButton(dijkstraButton, dijkstraButtonText, "Dijkstra", 720.f, 400.f, 95.f, 30.f);
    setupButton(aStarButton, aStarButtonText, "A*", 825.f, 400.f, 95.f, 30.f);
    setupButton(gbfsButton, gbfsButtonText, "GBFS", 720.f, 440.f, 95.f, 30.f);
    setupButton(startButton, startButtonText, "Start", 825.f, 440.f, 95.f, 30.f);
    setupButton(goalButton, goalButtonText, "Goal", 720.f, 500.f, 95.f, 30.f);
    setupButton(randomButton, randomButtonText, "Random", 825.f, 500.f, 95.f, 30.f);
    setupButton(compareButton, compareButtonText, "Compare", 720.f, 540.f, 95.f, 30.f);
    setupButton(clearButton, clearButtonText, "Clear", 825.f, 540.f, 95.f, 30.f);

    centerGrid();

    sidePanel.setSize(sf::Vector2f(UI_WIDTH, UI_HEIGHT));
    sidePanel.setPosition(GRID_WIDTH, 0.f);
    sidePanel.setFillColor(sf::Color(30, 30, 30));

    infoPanel.setSize(sf::Vector2f(220.f, 230.f));
    infoPanel.setPosition(GRID_WIDTH + 20.f, 110.f);
    infoPanel.setFillColor(sf::Color(42, 42, 42));
    infoPanel.setOutlineColor(sf::Color(90, 90, 90));
    infoPanel.setOutlineThickness(1.f);

    titleText.setFont(font);
    titleText.setString("PATH FINDING");
    titleText.setCharacterSize(28);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(GRID_WIDTH + 35.f, 28.f);

    algorithmText.setFont(font);
    algorithmText.setString("Algorithm: Ready");
    algorithmText.setCharacterSize(17);
    algorithmText.setFillColor(sf::Color::White);
    algorithmText.setPosition(GRID_WIDTH + 35.f, 135.f);

    stepsText.setFont(font);
    stepsText.setString("Steps: --");
    stepsText.setCharacterSize(17);
    stepsText.setFillColor(sf::Color::White);
    stepsText.setPosition(GRID_WIDTH + 35.f, 175.f);

    pathText.setFont(font);
    pathText.setString("Path: --");
    pathText.setCharacterSize(17);
    pathText.setFillColor(sf::Color::White);
    pathText.setPosition(GRID_WIDTH + 35.f, 215.f);

    timeText.setFont(font);
    timeText.setString("Time: --");
    timeText.setCharacterSize(17);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(GRID_WIDTH + 35.f, 255.f);

    controlsText.setFont(font);
    controlsText.setString(
        "KEYBOARD SHORTCUTS\n\n"
        "S: Start    G: Goal\n"
        "B: BFS      D: DFS\n"
        "K: Dijkstra A: A*\n"
        "F: GBFS     R: Random\n"
        "M: Compare  C: Clear"
    );
    controlsText.setCharacterSize(12);
    controlsText.setFillColor(sf::Color(180, 180, 180));
    controlsText.setPosition(GRID_WIDTH + 25.f, 600.f);

    compareText.setFont(font);
    compareText.setCharacterSize(16);
    compareText.setFillColor(sf::Color::White);
    compareText.setPosition(25.f, 690.f);
    compareText.setString("");
}

Game::~Game() = default;

void Game::run()
{
    while (window.isOpen() && isRunning)
    {
        processEvents();
        update();
        render();
    }
}

void Game::compareAlgorithms()
{
    std::ostringstream result;
    result << "COMPARISON\n\n";

    bool hasResult = false;

    if (compareBFS)
    {
        grid.clearPath();
        bfs.solve(grid);

        result << "BFS:        "
               << bfs.getSteps() << " steps  |  "
               << bfs.getPathLength() << " path  |  "
               << std::fixed << std::setprecision(3)
               << bfs.getExecutionTime() << " ms\n";
        hasResult = true;
    }

    if (compareDFS)
    {
        grid.clearPath();
        dfs.solve(grid);

        result << "DFS:        "
               << dfs.getSteps() << " steps  |  "
               << dfs.getPathLength() << " path  |  "
               << std::fixed << std::setprecision(3)
               << dfs.getExecutionTime() << " ms\n";
        hasResult = true;
    }

    if (compareDijkstra)
    {
        grid.clearPath();
        dijkstra.solve(grid);

        result << "Dijkstra:   "
               << dijkstra.getSteps() << " steps  |  "
               << dijkstra.getPathLength() << " path  |  "
               << std::fixed << std::setprecision(3)
               << dijkstra.getExecutionTime() << " ms\n";
        hasResult = true;
    }

    if (compareAStar)
    {
        grid.clearPath();
        aStar.solve(grid);

        result << "A*:         "
               << aStar.getSteps() << " steps  |  "
               << aStar.getPathLength() << " path  |  "
               << std::fixed << std::setprecision(3)
               << aStar.getExecutionTime() << " ms\n";
        hasResult = true;
    }

    if (compareGBFS)
    {
        grid.clearPath();
        gbfs.solve(grid);

        result << "GBFS:       "
               << gbfs.getSteps() << " steps  |  "
               << gbfs.getPathLength() << " path  |  "
               << std::fixed << std::setprecision(3)
               << gbfs.getExecutionTime() << " ms\n";
        hasResult = true;
    }

    if (!hasResult)
    {
        result << "No algorithm selected.";
    }

    compareText.setString(result.str());
}

void Game::update()
{
    if (isAnimating)
    {
        if (animationClock.getElapsedTime().asSeconds() >= animationDelay)
        {
            animationClock.restart();

            bool running = true;

            if (animationAlgorithm == AnimationAlgorithm::BFS)
            {
                running = bfs.step();
                stepsText.setString("Steps: " + std::to_string(bfs.getSteps()));
                pathText.setString("Path: " + std::to_string(bfs.getPathLength()));

                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << bfs.getExecutionTime();
                    timeText.setString("Time: " + timeStream.str() + " ms");
                }
            }
            else if (animationAlgorithm == AnimationAlgorithm::DFS)
            {
                running = dfs.step();
                stepsText.setString("Steps: " + std::to_string(dfs.getSteps()));
                pathText.setString("Path: " + std::to_string(dfs.getPathLength()));

                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << dfs.getExecutionTime();
                    timeText.setString("Time: " + timeStream.str() + " ms");
                }
            }
            else if (animationAlgorithm == AnimationAlgorithm::Dijkstra)
            {
                running = dijkstra.step();
                stepsText.setString("Steps: " + std::to_string(dijkstra.getSteps()));
                pathText.setString("Path: " + std::to_string(dijkstra.getPathLength()));

                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << dijkstra.getExecutionTime();
                    timeText.setString("Time: " + timeStream.str() + " ms");
                }
            }
            else if (animationAlgorithm == AnimationAlgorithm::A_star)
            {
                running = aStar.step();
                stepsText.setString("Steps: " + std::to_string(aStar.getSteps()));
                pathText.setString("Path: " + std::to_string(aStar.getPathLength()));

                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << aStar.getExecutionTime();
                    timeText.setString("Time: " + timeStream.str() + " ms");
                }
            }
            else if (animationAlgorithm == AnimationAlgorithm::GBFS)
            {
                running = gbfs.step();
                stepsText.setString("Steps: " + std::to_string(gbfs.getSteps()));
                pathText.setString("Path: " + std::to_string(gbfs.getPathLength()));

                if (!running)
                {
                    isAnimating = false;
                    std::ostringstream timeStream;
                    timeStream << std::fixed << std::setprecision(3)
                               << gbfs.getExecutionTime();
                    timeText.setString("Time: " + timeStream.str() + " ms");
                }
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!selectingStart && !selectingGoal && !isAnimating)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            Cell* cell = grid.getCellAt(
                static_cast<float>(mousePosition.x),
                static_cast<float>(mousePosition.y)
            );

            if (cell != nullptr &&
                cell->getState() != CellState::Start &&
                cell->getState() != CellState::Goal)
            {
                cell->setState(CellState::Wall);
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        if (!isAnimating)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            Cell* cell = grid.getCellAt(
                static_cast<float>(mousePosition.x),
                static_cast<float>(mousePosition.y)
            );

            if (cell != nullptr &&
                cell->getState() != CellState::Start &&
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
            isRunning = false;
            window.close();
            return;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::S)
            {
                if (isAnimating) continue;
                compareMode = false;
                selectingStart = true;
                selectingGoal = false;
                algorithmText.setString("Algorithm: Select Start");
            }
            else if (event.key.code == sf::Keyboard::G)
            {
                if (isAnimating) continue;
                compareMode = false;
                selectingStart = false;
                selectingGoal = true;
                algorithmText.setString("Algorithm: Select Goal");
            }
            else if (event.key.code == sf::Keyboard::B)
            {
                if (isAnimating) continue;
                compareMode = false;
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
            else if (event.key.code == sf::Keyboard::D)
            {
                if (isAnimating) continue;
                compareMode = false;
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
            else if (event.key.code == sf::Keyboard::K)
            {
                if (isAnimating) continue;
                compareMode = false;
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
            else if (event.key.code == sf::Keyboard::A)
            {
                if (isAnimating) continue;
                compareMode = false;
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
            else if (event.key.code == sf::Keyboard::F)
            {
                if (isAnimating) continue;
                compareMode = false;
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
            else if (event.key.code == sf::Keyboard::R)
            {
                if (isAnimating) continue;

                compareMode = false;
                selectingStart = false;
                selectingGoal = false;

                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<int> rowDist(11, 25);
                std::uniform_int_distribution<int> colDist(15, 35);

                rows = rowDist(gen);
                cols = colDist(gen);

                if (rows % 2 == 0) rows++;
                if (cols % 2 == 0) cols++;

                cellSize = std::min(
                    GRID_WIDTH / cols,
                    GRID_HEIGHT / rows
                );

                grid.resize(rows, cols, cellSize);
                centerGrid();
                mazeGenerator.generate(grid);

                algorithmText.setString("Algorithm: Random Maze");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
            }
            else if (event.key.code == sf::Keyboard::T)
            {
                if (isAnimating) continue;

                compareMode = false;
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

                if (emptyCells.size() >= 2)
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
            else if (event.key.code == sf::Keyboard::M)
            {
                if (isAnimating) continue;

                compareMode = false;
                selectingStart = false;
                selectingGoal = false;

                compareBFS = true;
                compareDFS = true;
                compareDijkstra = true;
                compareAStar = true;
                compareGBFS = true;

                compareAlgorithms();
                showComparison = true;
                algorithmText.setString("Algorithm: Compare");
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                compareMode = false;
                selectingStart = false;
                selectingGoal = false;

                compareBFS = false;
                compareDFS = false;
                compareDijkstra = false;
                compareAStar = false;
                compareGBFS = false;

                bfsButton.setFillColor(sf::Color(50, 50, 50));
                dfsButton.setFillColor(sf::Color(50, 50, 50));
                dijkstraButton.setFillColor(sf::Color(50, 50, 50));
                aStarButton.setFillColor(sf::Color(50, 50, 50));
                gbfsButton.setFillColor(sf::Color(50, 50, 50));
                compareButtonText.setString("Compare");
                showComparison = false;

                if (!isAnimating)
                    algorithmText.setString("Algorithm: Ready");
            }
            else if (event.key.code == sf::Keyboard::C)
            {
                isAnimating = false;
                selectingStart = false;
                selectingGoal = false;
                compareMode = false;

                compareBFS = false;
                compareDFS = false;
                compareDijkstra = false;
                compareAStar = false;
                compareGBFS = false;

                grid.clearPath();

                bfsButton.setFillColor(sf::Color(50, 50, 50));
                dfsButton.setFillColor(sf::Color(50, 50, 50));
                dijkstraButton.setFillColor(sf::Color(50, 50, 50));
                aStarButton.setFillColor(sf::Color(50, 50, 50));
                gbfsButton.setFillColor(sf::Color(50, 50, 50));
                compareButtonText.setString("Compare");

                algorithmText.setString("Algorithm: Ready");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
                compareText.setString("COMPARISON");
                showComparison = false;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (isAnimating)
                continue;

            sf::Vector2f mousePosition(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );

            // BFS
            if (event.mouseButton.button == sf::Mouse::Left &&
                bfsButton.getGlobalBounds().contains(mousePosition))
            {
                if (compareMode)
                {
                    compareBFS = !compareBFS;
                    bfsButton.setFillColor(
                        compareBFS ? sf::Color(70, 120, 70) : sf::Color(50, 50, 50)
                    );
                    continue;
                }

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
                continue;
            }

            // DFS
            if (event.mouseButton.button == sf::Mouse::Left &&
                dfsButton.getGlobalBounds().contains(mousePosition))
            {
                if (compareMode)
                {
                    compareDFS = !compareDFS;
                    dfsButton.setFillColor(
                        compareDFS ? sf::Color(70, 120, 70) : sf::Color(50, 50, 50)
                    );
                    continue;
                }

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
                continue;
            }

            // Dijkstra
            if (event.mouseButton.button == sf::Mouse::Left &&
                dijkstraButton.getGlobalBounds().contains(mousePosition))
            {
                if (compareMode)
                {
                    compareDijkstra = !compareDijkstra;
                    dijkstraButton.setFillColor(
                        compareDijkstra ? sf::Color(70, 120, 70) : sf::Color(50, 50, 50)
                    );
                    continue;
                }

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
                continue;
            }

            // A*
            if (event.mouseButton.button == sf::Mouse::Left &&
                aStarButton.getGlobalBounds().contains(mousePosition))
            {
                if (compareMode)
                {
                    compareAStar = !compareAStar;
                    aStarButton.setFillColor(
                        compareAStar ? sf::Color(70, 120, 70) : sf::Color(50, 50, 50)
                    );
                    continue;
                }

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
                continue;
            }

            // GBFS
            if (event.mouseButton.button == sf::Mouse::Left &&
                gbfsButton.getGlobalBounds().contains(mousePosition))
            {
                if (compareMode)
                {
                    compareGBFS = !compareGBFS;
                    gbfsButton.setFillColor(
                        compareGBFS ? sf::Color(70, 120, 70) : sf::Color(50, 50, 50)
                    );
                    continue;
                }

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
                continue;
            }

            // START
            if (event.mouseButton.button == sf::Mouse::Left &&
                startButton.getGlobalBounds().contains(mousePosition))
            {
                selectingStart = true;
                selectingGoal = false;
                compareMode = false;
                algorithmText.setString("Algorithm: Select Start");
                continue;
            }

            // GOAL
            if (event.mouseButton.button == sf::Mouse::Left &&
                goalButton.getGlobalBounds().contains(mousePosition))
            {
                selectingStart = false;
                selectingGoal = true;
                compareMode = false;
                algorithmText.setString("Algorithm: Select Goal");
                continue;
            }

            // RANDOM
            if (event.mouseButton.button == sf::Mouse::Left &&
                randomButton.getGlobalBounds().contains(mousePosition))
            {
                if (isAnimating)
                    continue;

                compareMode = false;
                selectingStart = false;
                selectingGoal = false;

                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<int> rowDist(11, 25);
                std::uniform_int_distribution<int> colDist(15, 35);

                rows = rowDist(gen);
                cols = colDist(gen);

                if (rows % 2 == 0) rows++;
                if (cols % 2 == 0) cols++;

                cellSize = std::min(
                    GRID_WIDTH / cols,
                    GRID_HEIGHT / rows
                );

                grid.resize(rows, cols, cellSize);
                centerGrid();
                mazeGenerator.generate(grid);

                algorithmText.setString("Algorithm: Random Maze");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
                continue;
            }

            // COMPARE
            if (event.mouseButton.button == sf::Mouse::Left &&
                compareButton.getGlobalBounds().contains(mousePosition))
            {
                if (!compareMode)
                {
                    compareMode = true;
                    selectingStart = false;
                    selectingGoal = false;

                    compareBFS = false;
                    compareDFS = false;
                    compareDijkstra = false;
                    compareAStar = false;
                    compareGBFS = false;

                    bfsButton.setFillColor(sf::Color(50, 50, 50));
                    dfsButton.setFillColor(sf::Color(50, 50, 50));
                    dijkstraButton.setFillColor(sf::Color(50, 50, 50));
                    aStarButton.setFillColor(sf::Color(50, 50, 50));
                    gbfsButton.setFillColor(sf::Color(50, 50, 50));

                    compareButtonText.setString("Run Compare");
                    algorithmText.setString("Compare Mode: Select Algorithms");
                }
                else
                {
                    bool anySelected =
                        compareBFS ||
                        compareDFS ||
                        compareDijkstra ||
                        compareAStar ||
                        compareGBFS;

                    if (anySelected)
                    {
                        compareAlgorithms();
                        showComparison = true;

                        compareMode = false;
                        compareButtonText.setString("Compare");
                        algorithmText.setString("Algorithm: Compare");

                        bfsButton.setFillColor(sf::Color(50, 50, 50));
                        dfsButton.setFillColor(sf::Color(50, 50, 50));
                        dijkstraButton.setFillColor(sf::Color(50, 50, 50));
                        aStarButton.setFillColor(sf::Color(50, 50, 50));
                        gbfsButton.setFillColor(sf::Color(50, 50, 50));

                        compareBFS = false;
                        compareDFS = false;
                        compareDijkstra = false;
                        compareAStar = false;
                        compareGBFS = false;
                    }
                    else
                    {
                        algorithmText.setString("Compare Mode: Select Algorithms");
                    }
                }

                continue;
            }

            // CLEAR
            if (event.mouseButton.button == sf::Mouse::Left &&
                clearButton.getGlobalBounds().contains(mousePosition))
            {
                isAnimating = false;
                selectingStart = false;
                selectingGoal = false;
                compareMode = false;

                compareBFS = false;
                compareDFS = false;
                compareDijkstra = false;
                compareAStar = false;
                compareGBFS = false;

                grid.clearPath();

                bfsButton.setFillColor(sf::Color(50, 50, 50));
                dfsButton.setFillColor(sf::Color(50, 50, 50));
                dijkstraButton.setFillColor(sf::Color(50, 50, 50));
                aStarButton.setFillColor(sf::Color(50, 50, 50));
                gbfsButton.setFillColor(sf::Color(50, 50, 50));

                compareButtonText.setString("Compare");
                algorithmText.setString("Algorithm: Ready");
                stepsText.setString("Steps: --");
                pathText.setString("Path: --");
                timeText.setString("Time: --");
                compareText.setString("COMPARISON");

                continue;
            }

            // GRID
            Cell* cell = grid.getCellAt(
                static_cast<float>(event.mouseButton.x),
                static_cast<float>(event.mouseButton.y)
            );

            if (cell != nullptr)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (selectingStart)
                    {
                        grid.setStart(cell);
                        selectingStart = false;
                        algorithmText.setString("Algorithm: Ready");
                    }
                    else if (selectingGoal)
                    {
                        grid.setGoal(cell);
                        selectingGoal = false;
                        algorithmText.setString("Algorithm: Ready");
                    }
                    else if (cell->getState() != CellState::Start &&
                             cell->getState() != CellState::Goal)
                    {
                        cell->setState(CellState::Wall);
                    }
                }
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
    window.clear(sf::Color(20, 20, 20));

    grid.draw(window);

    window.draw(sidePanel);
    window.draw(infoPanel);

    window.draw(titleText);
    window.draw(algorithmText);
    window.draw(stepsText);
    window.draw(pathText);
    window.draw(timeText);
    window.draw(controlsText);

    window.draw(bfsButton);
    window.draw(bfsButtonText);

    window.draw(dfsButton);
    window.draw(dfsButtonText);

    window.draw(dijkstraButton);
    window.draw(dijkstraButtonText);

    window.draw(aStarButton);
    window.draw(aStarButtonText);

    window.draw(gbfsButton);
    window.draw(gbfsButtonText);

    window.draw(startButton);
    window.draw(startButtonText);

    window.draw(goalButton);
    window.draw(goalButtonText);

    window.draw(randomButton);
    window.draw(randomButtonText);

    window.draw(compareButton);
    window.draw(compareButtonText);

    window.draw(clearButton);
    window.draw(clearButtonText);

    // COMPARISON POPUP
    if (showComparison)
    {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setPosition(0.f, 0.f);
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        const float panelWidth = 700.f;
        const float panelHeight = 400.f;
        const float panelX = (WINDOW_WIDTH - panelWidth) / 2.f;
        const float panelY = (WINDOW_HEIGHT - panelHeight) / 2.f;

        sf::RectangleShape panel;
        panel.setSize(sf::Vector2f(panelWidth, panelHeight));
        panel.setPosition(panelX, panelY);
        panel.setFillColor(sf::Color(35, 35, 35));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(sf::Color(150, 150, 150));
        window.draw(panel);

        sf::Text popupTitle;
        popupTitle.setFont(font);
        popupTitle.setString("ALGORITHM COMPARISON");
        popupTitle.setCharacterSize(24);
        popupTitle.setStyle(sf::Text::Bold);
        popupTitle.setFillColor(sf::Color::White);

        sf::FloatRect titleBounds = popupTitle.getLocalBounds();
        popupTitle.setPosition(
            panelX + (panelWidth - titleBounds.width) / 2.f - titleBounds.left,
            panelY + 25.f
        );
        window.draw(popupTitle);

        compareText.setCharacterSize(16);
        compareText.setFillColor(sf::Color::White);
        compareText.setPosition(panelX + 35.f, panelY + 85.f);
        window.draw(compareText);

        sf::Text closeText;
        closeText.setFont(font);
        closeText.setString("Press ESC or C to close");
        closeText.setCharacterSize(13);
        closeText.setFillColor(sf::Color(190, 190, 190));

        sf::FloatRect closeBounds = closeText.getLocalBounds();
        closeText.setPosition(
            panelX + (panelWidth - closeBounds.width) / 2.f - closeBounds.left,
            panelY + panelHeight - 35.f
        );
        window.draw(closeText);
    }

    window.display();
}

void Game::setupButton(
    sf::RectangleShape& button,
    sf::Text& text,
    const std::string& label,
    float x,
    float y,
    float width,
    float height)
{
    button.setSize(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(sf::Color(50, 50, 50));
    button.setOutlineThickness(1.f);
    button.setOutlineColor(sf::Color(120, 120, 120));

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);

    sf::FloatRect bounds = text.getLocalBounds();

    text.setPosition(
        x + (width - bounds.width) / 2.f - bounds.left,
        y + (height - bounds.height) / 2.f - bounds.top
    );
}
