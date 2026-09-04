#pragma once 
enum class CellState {
    Empty,
    Wall,
    Start,
    Goal,
    Visited,
    PathBFS,
    PathDFS, 
    PathDijkstra,
    PathAStar
};