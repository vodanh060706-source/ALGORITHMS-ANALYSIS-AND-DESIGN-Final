#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Grid.h"
#include "BFS.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "A_star.h"
#include "MazeGenerator.h"
class Game
{
private:
    // Kích thước cố định màn hình 
    static constexpr float WINDOW_WIDTH = 960.f;
    static constexpr float WINDOW_HEIGHT = 720.f;

    int rows;
    int cols;
    float cellSize;

    Grid grid;

    bool isRunning;
    sf::RenderWindow window;

    bool selectingStart = false;
    bool selectingGoal = false;

    BFS bfs;
    DFS dfs;
    Dijkstra dijkstra;
    A_star aStar;
    MazeGenerator mazeGenarator;

public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
};