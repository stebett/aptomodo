//
// Created by ginko on 20/03/24.
//

#include "astar.h"

#include <iostream>

// NODE
bool Node::operator==(const Node &otherNode) const {
    return this->x == otherNode.x && this->y == otherNode.y;
}

// Used by the closed set
bool Node::operator<(Node const &right) const {
    return (x < right.x) || (y < right.y);
}

void Node::draw(Color color) const {
    DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, color);
}

Node::Node() = default;

Node::Node(int x, int y) : x(x), y(y) {}


Node getTile(const Vector2 &position) {
    return {static_cast<int>(floor(position.x / tileSize)), static_cast<int>(floor(position.y / tileSize))};
}


// GRID
std::vector<Node> neighbors(const Map &grid, const Node node) {
    std::vector<Node> neighbors;
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++) {
            int neighbor_x = node.x - x;
            int neighbor_y = node.y - y;

            if (x == 0 && y == 0) continue;
            if (neighbor_x < 0 || neighbor_x > mapWidth / tileSize) continue;
            if (neighbor_y < 0 || neighbor_y > mapHeight / tileSize) continue;
            if (grid(neighbor_x, neighbor_y) == 1) continue;
            neighbors.emplace_back(neighbor_x, neighbor_y);
        }
    return neighbors;
}

Grid::Grid(Map matrix) : grid(matrix) {}

float manhattan(const Node &start, const Node &end) {
    return abs(end.x - start.x) + abs(end.y - start.y);
}


void Search::init(Node nodeStart, Node nodeEnd) {
    start = nodeStart;
    end = nodeEnd;
    closed[start] = 0.0f;
    open.push(std::pair(start, manhattan(start, end)));
};

void Search::reset() {
    open = {};
    closed.clear();
    came_from.clear();
    path.clear();
    frontier_idx = 0;
    start = {0, 0};
    end = {0, 0};
    completed = false;
}


void Search::step() {
    if (completed) {
        std::cout << "Search is already complete" << '\n';
        return;
    }

    std::cout << "Stepping..." << '\n';
    if (open.empty()) {
        std::cout << "Open list is empty" << '\n';
        completed = true;
        return;
    }
    current = open.top().first;
    if (current == end) {
        std::cout << "End found" << '\n';
        completed = true;
        getPath();
        return;
    }
    open.pop();
    std::cout << "Current \nx: " << current.x << "\ny: " << current.y << '\n';


    for (auto neighbor: neighbors(grid, current)) {
        if (!came_from.contains(neighbor)) {
            if (neighbor == start | neighbor == came_from[current]) continue;
            open.push(std::pair(neighbor, manhattan(neighbor, end)));
            came_from[neighbor] = current;
            closed[neighbor] = closed[current] + manhattan(neighbor, current);
            std::cout << "Node added ->   x: " << neighbor.x << "  y: " << neighbor.y << '\n';
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
    for (auto neighbor: neighbors(grid, current)) {
        if (!came_from.contains(neighbor)) {
            neighbor.draw({0, 250, 250, 120});
        }
    }

}

void Search::drawClosed() { for (auto [node, parent]: came_from) node.draw(RED); }

void Search::drawStart() { start.draw(GREEN); }

void Search::drawEnd() { end.draw(PURPLE); }

void Search::drawPath() {
    for (const Node &node: path) node.draw(DARKGREEN);
}


void Search::draw() {
//        drawOpen();
    drawClosed();
    drawStart();
    drawEnd();
    drawNeighbors();
    if (completed) { drawPath(); }
}


Search::Search(const Map &map) : grid(map) {}

