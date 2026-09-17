#pragma once 
enum class CellState {
    Empty,
    Wall,
    Start,
    Goal,
    PathBFS,
    PathGBFS,
    PathDFS, 
    Visited,
    PathDijkstra,
    VisitedSecondary, 
    Overlap,
    VisitedBFS,
    VisitedDFS,
    VisitedGBFS,
    VisitedDijkstra,
    VisitedAStar,
    PathAStar
};