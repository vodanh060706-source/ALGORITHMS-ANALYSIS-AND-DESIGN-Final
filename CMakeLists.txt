cmake_minimum_required(VERSION 3.16)

project(PathFindingGame)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

include_directories(include)

add_executable(PathFindingGame
    src/main.cpp
    src/Game.cpp
    src/Grid.cpp
    src/Cell.cpp
    src/Renderer.cpp
    src/BFS.cpp
    src/DFS.cpp
    src/Dijkstra.cpp
    src/A_star.cpp
    src/MazeGenerator.cpp
)

target_link_libraries(PathFindingGame
    sfml-graphics
    sfml-window
    sfml-system
)