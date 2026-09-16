#include "Dijkstra.h"
Dijkstra::Dijkstra() {}
int Dijkstra::getSteps() const
{
    return steps;
}
bool Dijkstra::solve(Grid& grid)
{
    steps = 0;
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    if (start == nullptr || goal == nullptr)
    {
        return false;
    }
    std::unordered_map<Cell*, int> distance;
    std::unordered_map<Cell*, Cell*> parent;
    std::unordered_set<Cell*> visited;
    using Node = std::pair<int, Cell*>;
    std::priority_queue<
        Node,
        std::vector<Node>,
        std::greater<Node>
    > pq;
    // Start
    distance[start] = 0;
    pq.push(
        std::make_pair(0, start)
    );
    while (!pq.empty())
    {
        std::pair<int, Cell*> node = pq.top();
        pq.pop();
        int currentDistance = node.first;
        Cell* current = node.second;
        if (visited.find(current) !=
            visited.end())
            {
                continue;
            }
        visited.insert(current);
        steps++;
        // Dijkstra = 4
        current->markVisited(4);
        if (current == goal)
        {
            break;
        }
        std::vector<Cell*> neighbors =
            grid.getNeighbors(current);
        for (Cell* neighbor : neighbors)
        {
            int newDistance = currentDistance + 1;
            if (distance.find(neighbor) == distance.end() ||
                newDistance < distance[neighbor])
            {
                distance[neighbor] = newDistance;
                parent[neighbor] = current;
                pq.push(std::make_pair(
                        newDistance, neighbor)
                );
            }
        }
    }
    if (distance.find(goal) == distance.end())
        {
            return false;
        }
    // Truy vết
    Cell* current = goal;
    while (current != start)
    {
        if (current != goal)
        {
            current->setState(CellState::PathDijkstra);
        }
        current = parent[current];
    }
    return true;
}