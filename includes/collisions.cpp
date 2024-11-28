//
// Created by ginko on 02/03/24.
//

#include "pch.h"
#include "collisions.h"

#include "managers/game.h"
#include "managers/levelManager.h"



bool CheckCollisionLineCircle(Vector2 start, Vector2 end, Vector2 center, float radius) {
    if (CheckCollisionPointCircle(end, center, radius)) return true;
    float distX = start.x - end.x;
    float distY = start.y - end.y;
    float len = sqrt((distX * distX) + (distY * distY));
    float dot = (((center.x - start.x) * (end.x - start.x)) + ((center.y - start.y) * (end.y - start.y))) / pow(len, 2);
    float closestX = start.x + (dot * (end.x - start.x));
    float closestY = start.y + (dot * (end.y - start.y));
    bool onSegment = CheckCollisionPointLine(Vector2{closestX, closestY}, start, end, 1);
    if (!onSegment) return false;
    distX = closestX - center.x;
    distY = closestY - center.y;
    float distance = sqrt((distX * distX) + (distY * distY));
    if (distance <= radius) {
        return true;
    }
    return false;
}

bool CheckTriangleContainsCircle(Vector2 center, Vector2 v1, Vector2 v2, Vector2 v3) {
    if ((((v2.y - v1.y) * (center.x - v1.x) - (v2.x - v1.x) * (center.y - v1.y)) <= 0)
        &&
        (((v3.y - v2.y) * (center.x - v2.x) - (v3.x - v2.x) * (center.y - v2.y)) <= 0)
        &&
        (((v1.y - v3.y) * (center.x - v3.x) - (v1.x - v3.x) * (center.x - v3.x)) <= 0))
        return true;
    return false;
}


bool CheckCollisionCircleTriangle(Vector2 center, float radius, Vector2 v1, Vector2 v2, Vector2 v3, float height) {
    if (!CheckCollisionCircles(center, radius, v1, height)) return false;
    if (CheckCollisionLineCircle(v1, v2, center, radius)) return true;
    if (CheckCollisionLineCircle(v1, v3, center, radius)) return true;
    if (CheckCollisionLineCircle(v2, v3, center, radius)) return true;
    if (CheckTriangleContainsCircle(center, v1, v2, v3)) return true;
    return false;
}


void solveCircleRecCollision(Vector2 &futurePos, float radius) {
    Vector2 upperBoundary = {std::max(0.0f, floor(futurePos.x / Const::tileSize) * Const::tileSize - 2 * Const::tileSize),
                             std::max(0.0f, floor(futurePos.y / Const::tileSize) * Const::tileSize - 2 * Const::tileSize)};
    Vector2 lowerBoundary = {std::min(float(Const::mapWidth), ceil(futurePos.x / Const::tileSize) * Const::tileSize + 2 * Const::tileSize),
                             std::min(float(Const::mapHeight), ceil(futurePos.y / Const::tileSize) * Const::tileSize + 2 * Const::tileSize)};
//    DrawRectangleV(upperBoundary, Vector2Subtract(lowerBoundary, upperBoundary), ColorAlpha(RED, 0.2));


    static float clampedX;
    static float clampedY;
    static float distanceX;
    static float distanceY;
    static float overlap;

    for (float x = upperBoundary.x; x <= lowerBoundary.x; x+=Const::tileSize) {
        for (float y = upperBoundary.y; y <= lowerBoundary.y; y+=Const::tileSize) {
            if (Game::grid.fromWorld(x, y) == IntValue::OBSTACLE) { // should also return whether it's out of map
                clampedX = std::max(x, std::min(futurePos.x, x + float(Const::tileSize)));
                clampedY = std::max(y, std::min(futurePos.y, y + float(Const::tileSize)));
                distanceX = clampedX - futurePos.x;
                distanceY = clampedY - futurePos.y;
                overlap = radius - Vector2Length({distanceX, distanceY});
//                std::cout << "Overlap: " << overlap << "\n";
//                std::cout << "Distance:" << Vector2Length({distanceX, distanceY}) << "\n\n";

                if (overlap > 0) {
                    futurePos = Vector2Subtract(futurePos, Vector2Scale(Vector2Normalize({distanceX, distanceY}), overlap));
                }
            }
        }
    }
}

