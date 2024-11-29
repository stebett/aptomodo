//
// Created by ginko on 17/04/24.
//


#include "renderingManager.h"

#include "../components.h"
#include "animationManager.h"
#include "attacks.h"
#include "game.h"
#include "levelManager.h"
#include "shadowCast.h"
#include "../config.h"
#include "../items.h"

namespace Rendering {
    void drawAttacks(entt::registry &registry) {
        auto effectView = registry.view<AttackEffect>();
        for (auto [entity, effect]: effectView.each()) {
            effect.Draw();
            if (effect.Expired()) {
                registry.destroy(entity);
            }
        }
    }

    void drawEnemyTexture(const entt::registry &registry, unsigned int frame) {
        auto livingView = registry.view<Living, ToRender, Radius, Position, LookAngle, Speed>();
        for (auto [entity, radius, position, rotation, speed]: livingView.each()) {
            DrawTexturePro(AnimationManager::getTexture("enemy/walking/v1/", frame * speed.actual),
                           {0, 0, 240, 240},
                           {position.x, position.y, 60, 60},
                           {30, 30},
                           rotation + 90,
                           WHITE);
        }
    }


    void drawAttacksBB(const entt::registry &registry) {
        registry.view<Attacks::Sword, b2BodyId>().each([](auto entity, auto sword, auto body) {
            auto pos = b2Body_GetPosition(body);
            b2ShapeId shape{};
            b2Body_GetShapes(body, &shape, 1);
            auto polygon = b2Shape_GetPolygon(shape);
            // DrawCircle(pos.x, pos.y, 5, RED);
            std::vector<Vector2> points;
            points.reserve(polygon.count + 1);
            for (int i{0}; i < polygon.count-1; i++) {
                auto [x, y] = polygon.vertices[i];
                auto [x2, y2] = polygon.vertices[i + 1];
                DrawLineEx({x + pos.x, y + pos.y}, {x2 + pos.x, y2 + pos.y}, 3, RED);
            }
            auto [x0, y0] = polygon.vertices[0];
            auto [xn, yn] = polygon.vertices[polygon.count-1];
            DrawLineEx({x0 + pos.x, y0 + pos.y}, {xn + pos.x, yn + pos.y}, 3, RED);
        });
    }


    void drawLivingBB(const entt::registry &registry) {
        auto livingView = registry.view<Living, ToRender, b2BodyId, Radius, Position, LookAngle, ColorBB>();
        for (auto [entity,body, radius , position, lookAngle, color]: livingView.each()) {
            auto pos = b2Body_GetPosition(body);
            b2ShapeId shape{};
            b2Body_GetShapes(body, &shape, 1);
            auto circle = b2Shape_GetCircle(shape);
            DrawCircle(pos.x, pos.y, circle.radius, color);
            DrawLineV(position, Vector2Add(
                          position, Vector2Scale(Vector2{cos(lookAngle * DEG2RAD), sin(lookAngle * DEG2RAD)}, 20.0f)),
                      BLACK);
        }
        if (Config::GetBool("draw_enemies_outside_screen")) {
            auto outsideScreen = registry.view<Living, Enemy, Radius, Position, LookAngle, ColorBB>(
                entt::exclude<ToRender>);
            for (auto [entity, radius, position, lookAngle, color]: outsideScreen.each()) {
                DrawCircleV(position, radius, BLACK);
                DrawLineV(position, Vector2Add(
                              position,
                              Vector2Scale(Vector2{cos(lookAngle * DEG2RAD), sin(lookAngle * DEG2RAD)}, 20.0f)),
                          BLACK);
            }
        }
    }

    void drawItems(const entt::registry &registry) {
        for (auto [entity, position]: registry.view<Item, ToRender, Position>().each()) {
            DrawRectangleV(position, {16, 16}, GOLD);
        }
    }

    /*
     * If player is close enough -> Pop message
     */
    void drawTooltips(const entt::registry &registry) {
        auto player = registry.view<Player>().front();
        auto playerPosition = registry.get<Position>(player);
        for (auto [entity, position]: registry.view<Item, Position>().each()) {
            if (Vector2Distance(playerPosition, position) < 40) {
                DrawText("Press F to pick up", position.x, position.y, 12, BLACK);
                return; // Only draw it for one item TODO why?
            }
        }
    }

    void drawProjectiles(const entt::registry &registry) {
        auto projectileView = registry.view<Projectile>();
        for (auto [entity, projectile]: projectileView.each()) {
            DrawCircleV(projectile.position, projectile.radius, DARKPURPLE);
            //        DrawCircleV(projectile.target, 5, RED);
        }
    }

    void DrawLevel(const Camera2D &camera) {
        // Don't really get why but it works
        const Vector2 cameraZero = GetScreenToWorld2D({0, 0}, camera);

        DrawTextureRec(Game::levelTexture,
                       {
                           cameraZero.x, -cameraZero.y - Const::screenHeight / camera.zoom,
                           Const::screenWidth / camera.zoom,
                           -Const::screenHeight / camera.zoom
                       },
                       cameraZero, WHITE);

        // DrawTextureRec(LevelManager::renderedLevelTexture,
        //                    {0, 0, (float) LevelManager::renderedLevelTexture.width, (float) -LevelManager::renderedLevelTexture.height},
        //                    {0, 0}, WHITE);
    }

    void drawLevelCollisions() {
        for (auto row = 0; row < IntGrid::rows(); row++) {
            for (auto col = 0; col < IntGrid::cols(); col++) {
                Color color = {255, 255, 255, 0};
                if (Game::grid(row, col)[IntValue::OBSTACLE])
                    color = {255, 0, 0, 125};
                if (Game::grid(row, col)[IntValue::NEAR_OBSTACLE])
                    color = {255, 0, 0, 50};
                if (Game::grid(row, col)[IntValue::NPC])
                    color = {255, 255, 0, 50};
                DrawRectangle(row * Const::tileSize, col * Const::tileSize, Const::tileSize, Const::tileSize, color);
            }
        }
    }

    // TODO DrawSplineCatmullRom
    void drawEnemyExtra(const entt::registry &registry) {
        auto selectedView = registry.view<Living, ToRender, Selected, Radius, Position, LookAngle, ColorBB, Path, Target
            , Chasing>();
        for (auto [entity, radius, position, lookAngle, color, path, target, chasing]: selectedView.each()) {
            DrawCircleV(position, radius + 2, PURPLE);

            if (Config::GetBool("show_enemy_fov")) {
                const auto isChasing = chasing.isChasing();
                const auto hearRange = isChasing
                                           ? Config::GetFloat("enemyHearRangeChasing")
                                           : Config::GetFloat("enemyHearRange");
                const auto sightRange = isChasing
                                            ? Config::GetFloat("enemySightRangeChasing")
                                            : Config::GetFloat("enemySightRange");
                const auto colorRange = isChasing ? ColorAlpha(RED, 0.1) : ColorAlpha(WHITE, 0.1);
                // DrawCircleSector(position, sightRange, lookAngle - 91.0f, lookAngle + 91.0f, 2, colorRange);
                DrawCircleV(position, hearRange, colorRange);
                visualizeFOV(Game::grid, position.x, position.y, sightRange, lookAngle - 91.0f, lookAngle + 91.0f);
            }

            if (Config::GetBool("show_astar_path")) {
                auto points = path.path;
                constexpr float alpha = 0.9;
                constexpr float astarRadius = 2;
                constexpr auto astarColor = Color{255, 255, 255, 255};
                for (size_t i = 0; i < path.indexMax; ++i) {
                    DrawCircleV({points[i].x, points[i].y}, astarRadius, ColorAlpha(astarColor, alpha));
                }
                // DrawSplineCatmullRom(points.data(), path.indexMax, 1.0f, ColorAlpha(astartColor, alpha));
                DrawCircleV(target, astarRadius + 0.5, ColorAlpha(astarColor, alpha));
            }
        }
    }


    void Draw(entt::registry &registry, const Camera2D &camera, const unsigned int frame) {
        drawItems(registry);

        drawEnemyExtra(registry);
        if (Config::GetBool("show_bounding_box")) drawLivingBB(registry);
        if (Config::GetBool("show_enemy_texture"))
            drawEnemyTexture(
                registry, frame / Config::GetInt("enemy_walking_animation_fps"));
        if (Config::GetBool("show_attacks")) drawAttacks(registry);
        if (Config::GetBool("draw_level_collisions")) drawLevelCollisions();

        drawProjectiles(registry);
        drawAttacksBB(registry);
        drawTooltips(registry);
    }
}
