#pragma once

#include <SFML/Graphics.hpp>
#include "Grid.h"
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
public:
    Game();
    ~Game();
    void run();


};