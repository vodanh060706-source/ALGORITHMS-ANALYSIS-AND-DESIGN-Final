#include "Dijkstra.h"
Dijkstra::Dijkstra(){};
bool Dijkstra::solve(Grid& grid) {
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();

    if (start == nullptr || goal == nullptr)
    {
        return false;
    }
    // Khoảng cách ngắn nhất từ Start -> Cell
    std::unordered_map<Cell*, int> distance;
    // Lưu lại các ô cha đã đi qua   
    std::unordered_map<Cell*, Cell*> parent;
    // Lưu lại các ô đã được xử lí 
    std::unordered_set<Cell*> visited; 
    // Priority_queue lưu các pair<distance, Cell>
    using Node = std::pair<int, Cell*>;
    std::priority_queue<Node,
                        std::vector<Node>, 
                        std::greater<Node>
                    > pq;
    // Đưa start vào hàng đợi ưu tiên 
    pq.push({0, start});
    while(!pq.empty()) {
        std::pair<int, Cell*> node = pq.top();
        pq.pop();
        int currentDistance = node.first;
        Cell* current = node.second; 
        // Nếu xử lí rồi bỏ qua 
        if (visited.find(current) != visited.end())
            continue;
        visited.insert(current);
        // Đã tới Goal 
        if(current == goal) break;
        // Lấy ô hàng xóm 
        std::vector<Cell*> neighbors = grid.getNeighbors(current);
        for(Cell* neighbor : neighbors) {
            int newDistance = currentDistance + 1;
            // Nếu chưa có thì tìm đường đi ngắn hơn 
            if(distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) 
            {
                distance[neighbor] = newDistance;
                parent[neighbor] = current;
                    pq.push({newDistance, neighbor});
            }
        }   
    }
    // Không tìm thấy Goal 
    if(distance.find(goal) == distance.end()) {
        return false;
    }
    // Truy vết đường đi
    Cell* current = goal;
    while(current != start) {
        current->setState(CellState::PathDijkstra);
        current = parent[current];
    }
    return true;
}