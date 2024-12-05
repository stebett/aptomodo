//
// Created by ginko on 20/03/24.
//

#include "astar.h"

#include <utility>
#include "managers/game.h"

// NODE
bool Node::operator==(const Node &otherNode) const {
    return this->x == otherNode.x && this->y == otherNode.y;
}

// Used by the closed set
bool Node::operator<(Node const &right) const {
    return (x < right.x) || (y < right.y);
}

void Node::draw(Color color) const {
    DrawRectangle(x * Const::tileSize, y * Const::tileSize, Const::tileSize, Const::tileSize, color);
}

Node::Node() = default;

Node::Node(int x, int y) : x(x), y(y) {
}

Node::Node(const Vector2 vec) : x(static_cast<int>(vec.x)), y(static_cast<int>(vec.y)) {
}


Node getTile(const Vector2 &position) {
    return {
            static_cast<int>(floor(position.x / Const::tileSize)), static_cast<int>(floor(position.y / Const::tileSize))
    };
}


std::size_t std::hash<Node>::operator()(const Node &k) const {
    using std::size_t;
    using std::hash;
    using std::string;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<int>()(k.x)
             ^ (hash<int>()(k.y) << 1)) >> 1);
}

bool CompareNodes::operator()(const std::pair<Node, float> &a, const std::pair<Node, float> &b) const {
    return a.second > b.second;
}


// GRID
std::vector<Node> neighbors(const Node node) {
    std::vector<Node> neighbors;
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++) {
            int neighbor_x = node.x - x;
            int neighbor_y = node.y - y;

            if (x == 0 && y == 0) continue;
            if (neighbor_x < 0 || neighbor_x >= IntGrid::rows()) continue;
            if (neighbor_y < 0 || neighbor_y >= IntGrid::cols()) continue;
            if (Game::grid.safe(neighbor_x, neighbor_y)[IntValue::OBSTACLE]) continue;
            neighbors.emplace_back(neighbor_x, neighbor_y);
        }
    return neighbors;
}

float manhattan(const Node &start, const Node &end) {
    return abs(end.x - start.x) + abs(end.y - start.y);
}


void Search::init(Node nodeStart, Node nodeEnd) {
    start = nodeStart;
    end = nodeEnd;
    closed[start] = 0.0f;
    open.emplace(start, manhattan(start, end));
}

void Search::setFree(std::unordered_set<Node> nodes) {
    forcedFree = std::move(nodes);
}

void Search::reset() {
    open = {};
    closed.clear();
    came_from.clear();
    path.clear();
    frontier_idx = 0;
    start = {0, 0};
    end = {0, 0};
    completed = false;
    stepNumber = 0;
}


void Search::step() {
    stepNumber++;
    //    if (stepNumber > stepLimit) completed = true;
    if (completed) {
        //        std::cout << "Search is already complete" << '\n';
        return;
    }

    //    std::cout << "Stepping..." << '\n';
    if (open.empty()) {
        //        std::cout << "Open list is empty" << '\n';
        completed = true;
        return;
    }
    current = open.top().first;
    if (current == end) {
        //        std::cout << "End found" << '\n';
        completed = true;
        getPath();
        return;
    }
    open.pop();
    //    std::cout << "Current \nx: " << current.x << "\ny: " << current.y << '\n';


    for (auto neighbor: neighbors(current)) {
        if (!came_from.contains(neighbor)) {
            if (neighbor == start | neighbor == came_from[current]) continue;
            float terrainPenalty = 3.0f * static_cast<float>(
                    Game::grid.safe(neighbor.x, neighbor.y)[IntValue::NEAR_OBSTACLE]);
            if (!forcedFree.contains(neighbor) && Game::grid.safe(neighbor.x, neighbor.y)[IntValue::NPC])
                terrainPenalty += 20.0f;
            open.emplace(neighbor, manhattan(neighbor, end) + terrainPenalty);
            came_from[neighbor] = current;
            closed[neighbor] = closed[current] + manhattan(neighbor, current) + terrainPenalty;
            //            std::cout << "Node added ->   x: " << neighbor.x << "  y: " << neighbor.y << '\n';
        }
    }
}

//    void drawOpen() { for (const Node &node: open) node.draw(YELLOW); }
void Search::getPath() {
    Node parent = current;
    while (true) {
        path.push_back(parent);
        if (!came_from.contains(parent)) break;
        if (parent == start) break;
        parent = came_from[parent];
    }
    std::reverse(path.begin(), path.end());
}

void Search::drawNeighbors() {
    Node parent = current;
    while (true) {
        parent.draw({250, 250, 0, 120});
        if (!came_from.contains(parent)) break;
        if (parent == start) break;
        parent = came_from[parent];
    }
    for (auto neighbor: neighbors(current)) {
        if (!came_from.contains(neighbor)) {
            neighbor.draw({0, 250, 250, 120});
        }
    }
}

void Search::drawClosed() { for (auto [node, parent]: came_from) node.draw(ColorAlpha(RED, 0.15)); }

void Search::drawStart() { start.draw(ColorAlpha(GREEN, 0.15)); }

void Search::drawEnd() { end.draw(ColorAlpha(PURPLE, 0.15)); }

void Search::drawPath() {
    for (const Node &node: path) node.draw(ColorAlpha(DARKGREEN, 0.15));
}

void Search::exportPath(Path &pathToUpdate) {
    const int size = std::min(path.size() - 1, pathToUpdate.path.max_size());
    if (path.empty()) {
        return;
    }

    for (int i = 0; i < size; i++) {
        pathToUpdate.path[i] = {
                static_cast<float>(path[i + 1].x) * Const::tileSize + static_cast<float>(Const::tileSize) / 2,
                static_cast<float>(path[i + 1].y) * Const::tileSize + static_cast<float>(Const::tileSize) / 2
        };
    }
    pathToUpdate.index = 0;
    pathToUpdate.indexMax = size;
    pathToUpdate.validate();
}


void Search::draw() {
    //        drawOpen();
    drawClosed();
    drawStart();
    drawEnd();
    //    drawNeighbors();
    if (completed) { drawPath(); }
}
