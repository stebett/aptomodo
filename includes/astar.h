//
// Created by ginko on 20/03/24.
//

#ifndef ASTAR_H
#define ASTAR_H

#include <unordered_map>
#include <queue>
#include <set>
#include <raylib.h>
#include <boost/container_hash/hash.hpp>
#include "constants.h"


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

};

Node getTile(const Vector2 &position);

template<>
struct std::hash<Node> {
    std::size_t operator()(const Node &k) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, k.x);
        boost::hash_combine(seed, k.y);
        return seed;
    }
};



class Grid {
    Map grid;

public:
    [[nodiscard]] std::vector<Node> neighbors(Node node) const;

    explicit Grid(Map matrix);
};

using EvaluatedNode= std::pair<Node, float> ;
static auto f = [](EvaluatedNode node1, EvaluatedNode node2) {return node1.second > node2.second;};
class Search {

    const Map &grid;
    std::priority_queue<EvaluatedNode, std::vector<EvaluatedNode>, decltype(f)> open;
    unsigned int frontier_idx = 0;
    std::unordered_map<Node, float> closed{};
    std::unordered_map<Node, Node> came_from;
    Node start;
    Node end;
    Node current;
    int stepNumber;
    int stepLimit = 100;

public:
    bool completed = false;
    std::vector<Node> path;

    void init(Node nodeStart, Node nodeEnd);
    void reset();
    void step();

    void getPath();

    void drawNeighbors();

    void drawClosed();

    void drawStart();

    void drawEnd();

    void drawPath();

    void draw();

    Search(const Map &map);


    Path exportPath();

    void exportPath(Path &pathToUpdate);
};


#endif //ASTAR_H
