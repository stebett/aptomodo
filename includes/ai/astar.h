//
// Created by ginko on 20/03/24.
//

#ifndef ASTAR_H
#define ASTAR_H

#include "components.h"

class Node {
public:
    // position in the grid (NOT IN THE WORLD)
    int x;
    int y;

    bool operator==(const Node &otherNode) const;

    bool operator<(Node const &right) const;

    void draw(Color color = YELLOW) const;

    Node();

    Node(int x, int y);

    explicit Node(Vector2 vec);
};

Node getTile(const Vector2 &position);

template<>
struct std::hash<Node> {
    std::size_t operator()(const Node &k) const;
};

using EvaluatedNode = std::pair<Node, float>;

struct CompareNodes {
    bool operator()(const std::pair<Node, float>& a, const std::pair<Node, float>& b) const;
};

// TODO use this directly instead of Path
// TODO Integrate path in a spline
class Search {
    std::priority_queue<EvaluatedNode, std::vector<EvaluatedNode>, CompareNodes> open;
    unsigned int frontier_idx = 0;
    std::unordered_map<Node, float> closed{};
    std::unordered_map<Node, Node> came_from;
    Node start;
    Node end;
    Node current;
    std::unordered_set<Node> forcedFree;
    int stepNumber = 0;
    int stepLimit = 100;

public:
    bool completed = false;
    std::vector<Node> path;

    void init(Node nodeStart, Node nodeEnd);

    void setFree(std::unordered_set<Node> nodes);

    void reset();

    void step();

    void getPath();

    void drawNeighbors();

    void drawClosed();

    void drawStart();

    void drawEnd();

    void drawPath();

    void draw();

    Path exportPath();

    void exportPath(Path &pathToUpdate);
};


#endif //ASTAR_H
