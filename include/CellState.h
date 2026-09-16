#pragma once 
enum class CellState {
    Empty,
    Wall,
    Start,
    Goal,
    PathBFS,
    PathDFS, 
    Visited,
    PathDijkstra,
    VisitedSecondary, 
    Overlap,
    VisitedBFS,
    VisitedDFS,
    VisitedDijkstra,
    VisitedAStar,
    PathAStar
};