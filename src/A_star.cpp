#include "A_star.h"
A_star::A_star() {}
bool A_star::solve(Grid& grid) {
    Cell* start = grid.getStart();
    Cell* goal = grid.getGoal();
    // Không có start và goal 
    if(start == nullptr || goal == nullptr) {
        return false;
    }
    // gScore = Chi phí từ ô Start đến Goal 
    std::unordered_map<Cell*, int> gScore;
    // parent: lưu giữ ô cha của mỗi Cell 
    std::unordered_map<Cell*, Cell*> parent;
    // Các ô đã xử lí 
    std::unordered_set<Cell*> process;
    // Node lưu fScore, Cell 
    using Node = std::pair<int, Cell*>;
    std::priority_queue<Node, 
                        std::vector<Node>, 
                        std::greater<Node>> open;
    // Chi phí ban đầu bằng 0 
    gScore[start] = 0;
    //f[start] = g[start] + h[start] 
    int hStart = std::abs(start->getRow() - goal->getRow()) 
                + std::abs(start->getCol() - goal->getCol());
    open.push({hStart, start});
    while(!open.empty()) {
        // Lấy node f nhỏ nhất 
        Node node = open.top();
        open.pop();
        int currentF = node.first;
        Cell* current = node.second;
        // Nếu đã xử lí thì bỏ qua 
        if(process.find(current) != process.end()) 
        continue;
        process.insert(current);
        // Tới Goal 
        if(current == goal) break;
        // Lấy hàng xóm 
        std::vector<Cell*> neighbors = grid.getNeighbors(current);
        for(Cell* neighbor : neighbors) {
            // nếu xử lí thì bỏ qua 
            if(process.find(neighbor) != process.end()) continue;
            // Mỗi bước đi có cost bằng 1
            int tentativeG = gScore[current]+1;
            // Nếu chưa có hoặc tìm đường đi tốt hơn
            if(gScore.find(neighbor) == gScore.end() || tentativeG < gScore[neighbor]) {
                // Cập nhật điểm 
                gScore[neighbor] = tentativeG;
                // Lưu cha 
                parent[neighbor] = current;
                  // Heuristic Manhattan
                int h_neighbor = std::abs(neighbor->getRow() - goal->getRow()) 
                               + std::abs(neighbor->getCol() - goal->getCol());
                // f(neighbor) = g(neighbor) + h(neighbor)
                int f_neighbor = tentativeG + h_neighbor;
                open.push({f_neighbor, neighbor});
            }
        } 
    }
    // Không tìm thấy Goal 
    if(gScore.find(goal)==gScore.end()) return false;
    // Truy vết đường đi 
    Cell* current = goal;
    while(current != start) {
        current->setState(CellState::PathAStar);
        current = parent[current];
    }
    return true;
}