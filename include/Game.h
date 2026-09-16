#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>
#include "Grid.h"
#include "BFS.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "A_star.h"
#include "MazeGenerator.h"

class Game
{
private:
    void processEvents();
    void update();
    void render();

    Grid grid;

    bool isRunning;
    sf::RenderWindow window;

    bool selectingStart = false;
    bool selectingGoal = false;

    // Kích thước cửa sổ
    static constexpr float WINDOW_WIDTH = 960.f;
    static constexpr float WINDOW_HEIGHT = 820.f;

    // Khu vực Grid
    static constexpr float GRID_WIDTH = 700.f;
    static constexpr float GRID_HEIGHT = 720.f;

    // Khu vực UI
    static constexpr float UI_WIDTH = 260.f;
    static constexpr float UI_HEIGHT = 820.f;

    int rows;
    int cols;
    float cellSize;

    BFS bfs;
    DFS dfs;
    Dijkstra dijkstra;
    A_star aStar;
    MazeGenerator mazeGenerator;

    // Font
    sf::Font font;

    // Text
    sf::Text titleText;
    sf::Text algorithmText;
    sf::Text stepsText;
    sf::Text pathText;
    sf::Text timeText;
    sf::Text controlsText;
    // Panel
    sf::RectangleShape sidePanel;
    sf::RectangleShape infoPanel;

public:
    Game();
    ~Game();
    void centerGrid();
    void run();
};