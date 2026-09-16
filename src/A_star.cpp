#include "A_star.h"

#include <cmath>
A_star::A_star() {}
int A_star::getSteps() const
{
    return steps;
}
namespace 
{
int heuristic(
    Cell* current,
    Cell* goal
)
{
    return std::abs(current->getRow()- goal->getRow())
         + std::abs(current->getCol() - goal->getCol());
}
}
bool A_star::solve(Grid& grid)
{
    steps = 0;
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    if (start == nullptr || goal == nullptr)
    {
        return false;
    }
    std::unordered_map<Cell*, int> gScore;
    std::unordered_map<Cell*, Cell*> parent;
    std::unordered_set<Cell*> visited;
    using Node = std::pair<int, Cell*>;

    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > pq;
    // g(start) = 0
    gScore[start] = 0;
    int h = heuristic(start, goal);
    int f = h;
    pq.push(
        std::make_pair(f, start)
    );
    while (!pq.empty())
    {
        std::pair<int, Cell*> node = pq.top();
        pq.pop();
        Cell* current =
            node.second;
        if (visited.find(current) !=
            visited.end())
        {
            continue;
        }
        visited.insert(current);
        steps++;                            
        // A* = 8
        current->markVisited(8);
        if (current == goal)
        {
            break;
        }
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            int newG = gScore[current] + 1;
            if (gScore.find(neighbor) == gScore.end() ||
                newG < gScore[neighbor])
            {
                gScore[neighbor] = newG;
                parent[neighbor] = current;
                int h = heuristic(neighbor, goal);
                int f = newG + h;
                pq.push(std::make_pair(f,neighbor));
            }
        }
    }
    if (gScore.find(goal) == gScore.end())
        {
            return false;
        }
    // Truy vết
    Cell* current = goal;
    while (current != start)
    {
        if (current != goal)
        {
            current->setState(CellState::PathAStar);
        }
        current = parent[current];
    }
    return true;
}