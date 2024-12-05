//
// Created by ginko on 11/26/24.
//

#include "space.h"

#include <components.h>
#include <constants.h>
#include <items.h>
#include <managers/game.h>

constexpr int simPad{700};
constexpr Rectangle renderSpace{0, 0, Const::screenWidth, Const::screenHeight};
constexpr Rectangle simulationSpace{-simPad, -simPad, Const::screenWidth + simPad, Const::screenHeight + simPad};

bool pointInRec(const Vector2 &point, const Rectangle &rec) {
    return (point.x >= rec.x) && (point.x < rec.width) && (point.y >= rec.y) && (point.y < rec.height);
}

bool inRenderSpace(const Vector2 &point) {
    return pointInRec(point, renderSpace);
}

bool inSimSpace(const Vector2 &point) {
    return pointInRec(point, simulationSpace);
}

void emplaceStuff(const entt::entity entity, const Vector2 &position,
                  const Camera2D &camera) {
    const Vector2 screenPos = GetWorldToScreen2D(position, camera);
    if (!inSimSpace(screenPos)) {
        Game::registry.remove<ToRender>(entity);
        Game::registry.remove<ToSimulate>(entity);
    } else {
        Game::registry.emplace_or_replace<ToSimulate>(entity);
        if (inRenderSpace(screenPos)) {
            Game::registry.emplace_or_replace<ToRender>(entity);
        }
    }
}

namespace Space {
    std::vector<Vector2> tilesOccupied(const Vector2 center, const float radius) {
        std::vector<Vector2> occupiedTiles;
        const int top = ceil(center.y - radius) / Const::tileSize;
        const int bottom = floor(center.y + radius) / Const::tileSize;
        const int left = ceil(center.x - radius) / Const::tileSize;
        const int right = floor(center.x + radius) / Const::tileSize;

        for (int y = top; y <= bottom; y++) {
            for (int x = left; x <= right; x++) {
                const auto point = Vector2{static_cast<float>(x), static_cast<float>(y)};
                if (CheckCollisionCircleRec(center, radius,
                                            {
                                                    point.x * Const::tileSize,
                                                    point.y * Const::tileSize,
                                                    Const::tileSize,
                                                    Const::tileSize
                                            })) {
                    occupiedTiles.emplace_back(point);
                }
            }
        }
        return occupiedTiles;
    }


    void Update(const Camera2D &camera) {
        static std::vector<Vector2> occupied{}; // TODO make this a set
        for (auto tile: occupied) Game::grid.setFree(tile);
        occupied.clear();

        Game::registry.view<Item, Position>().each([&camera](auto entity, auto position) {
            emplaceStuff(entity, position, camera);
        });
        Game::registry.view<Enemy, Position, Radius>().each([&camera](auto entity, auto position, auto radius) {
            emplaceStuff(entity, position, camera);
            for (auto tile: tilesOccupied(position, radius))
                occupied.emplace_back(tile);
        });
        for (auto tile: occupied) Game::grid.setOccupied(tile);
    }
}
