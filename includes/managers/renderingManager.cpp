//
// Created by ginko on 17/04/24.
//

#include "audioManager.h"
#include "renderingManager.h"
#include "../components.h"
#include "animationManager.h"
#include "levelManager.h"
#include "shadowCast.h"
#include "../config.h"
#include "../items.h"


RenderingManager *RenderingManager::instance;
entt::registry *RenderingManager::m_registry;

RenderingManager &RenderingManager::Instance() {
    return *instance;
}


RenderingManager::RenderingManager(entt::registry &registry) {
    m_registry = &registry;
}

void RenderingManager::UpdateCamera(Camera2D &camera, const Position &playerPosition) {
    float target_x = playerPosition.x;
    float target_y = playerPosition.y;

    // Don't show map end
    if (playerPosition.x < screenWidth / (2 * camera.zoom)) {
        target_x = screenWidth / (2 * camera.zoom);
    }
    if (playerPosition.x > mapWidth - screenWidth / (2 * camera.zoom)) {
        target_x = mapWidth - screenWidth / (2 * camera.zoom);
    }
    if (playerPosition.y < screenHeight / (2 * camera.zoom)) {
        target_y = screenHeight / (2 * camera.zoom);
    }
    if (playerPosition.y > mapHeight - screenHeight / (2 * camera.zoom)) {
        target_y = mapHeight - screenHeight / (2 * camera.zoom);
    }

    camera.target = Vector2Lerp(camera.target, {target_x, target_y}, 0.1);
}


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
    auto livingView = registry.view<Living, Radius, Position, LookAngle, Speed>();
    for (auto [entity, radius, position, rotation, speed]: livingView.each()) {
        DrawTexturePro(AnimationManager::getTexture("enemy/walking/v1/", frame * speed.actual),
                       {0, 0, 240, 240},
                       {position.x, position.y, 60, 60},
                       {30, 30},
                       rotation + 90,
                       WHITE);
    }
}

void drawLivingBB(const entt::registry &registry) {
    auto livingView = registry.view<Living, Radius, Position, LookAngle, ColorBB>();
    for (auto [entity, radius, position, lookAngle, color]: livingView.each()) {
        DrawCircleV(position, radius, color);
        DrawLineV(position, Vector2Add(
                      position, Vector2Scale(Vector2{cos(lookAngle * DEG2RAD), sin(lookAngle * DEG2RAD)}, 20.0f)),
                  BLACK);
    }
}

void drawItems(const entt::registry &registry) {
    for (auto [entity, position]: registry.view<Item, Position>().each()) {
        DrawRectangleV(position, {16, 16}, GOLD);
    }
}

/*
 * If player is close enough -> Pop message
 */
void drawTooltips(const entt::registry &registry) {
    auto player = registry.view<Player>().front();
    auto playerPosition = registry.get<Position>(player); // This could be static, or a static ref
    for (auto [entity, position]: registry.view<Item, Position>().each()) {
        if (Vector2Distance(playerPosition, position) < 20) {
            DrawText("Press F to pick up", position.x, position.y, 12, BLACK);
            return; // Only draw it for one item
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

void RenderingManager::DrawLevel(const Camera2D &camera) {
    // Don't really get why but it works
    const Vector2 cameraZero = GetScreenToWorld2D({0, 0}, camera);

    DrawTextureRec(LevelManager::renderedLevelTexture,
                   {
                       cameraZero.x, -cameraZero.y - screenHeight / camera.zoom, screenWidth / camera.zoom,
                       -screenHeight / camera.zoom
                   },
                   cameraZero, WHITE);

    // DrawTextureRec(LevelManager::renderedLevelTexture,
    //                    {0, 0, (float) LevelManager::renderedLevelTexture.width, (float) -LevelManager::renderedLevelTexture.height},
    //                    {0, 0}, WHITE);
}

void drawLevelCollisions() {
    for (auto row = 0; row < LevelManager::grid.rows(); row++) {
        for (auto col = 0; col < LevelManager::grid.cols(); col++) {
            Color color = {255, 255, 255, 0};
            if (LevelManager::grid(row, col) == IntValue::OBSTACLE)
                color = {255, 0, 0, 125};
            if (LevelManager::grid(row, col) == IntValue::NEAR_OBSTACLE)
                color = {255, 0, 0, 50};
            if (LevelManager::grid(row, col) == IntValue::NPC)
                color = {255, 255, 0, 50};
            DrawRectangle(row * tileSize, col * tileSize, tileSize, tileSize, color);
        }
    }
}

void drawEnemyExtra(const entt::registry &registry) {
    auto selectedView = registry.view<Living, Selected, Radius, Position, LookAngle, ColorBB, Path, Target, Chasing>();
    for (auto [entity, radius, position, lookAngle, color, path, target, chasing]: selectedView.each()) {
        DrawCircleV(position, radius + 2, PURPLE);

        if (config::show_enemy_fov) {
            const auto isChasing = chasing.isChasing();
            const auto hearRange = isChasing ? config::enemyHearRangeChasing : config::enemyHearRange;
            const auto sightRange = isChasing ? config::enemySightRangeChasing : config::enemySightRange;
            const auto colorRange = isChasing ? ColorAlpha(RED, 0.1) : ColorAlpha(WHITE, 0.1);
            DrawCircleSector(position, sightRange, lookAngle - 91.0f, lookAngle + 91.0f, 2, colorRange);
            DrawCircleV(position, hearRange, colorRange);
            do_fov(LevelManager::grid, position.x, position.y, sightRange, lookAngle - 91.0f, lookAngle + 91.0f);
        }

        if (config::show_astar_path) {
            // Draw lines connecting points
            auto points = path.path;
            for (size_t i = 0; i < path.indexMax - 1; ++i) {
                Vector2 p1 = {points[i].x, points[i].y};
                Vector2 p2 = {points[i + 1].x, points[i + 1].y};
                DrawLineV(p1, p2, BLACK);
            }

            for (size_t i = 0; i < path.indexMax; ++i) {
                DrawCircleV({points[i].x, points[i].y}, 2, YELLOW);
            }
            DrawCircleV(target, 2, ORANGE);
        }
    }
}


void RenderingManager::Draw(const Camera2D &camera, const unsigned int frame) {
    drawItems(*m_registry);
    drawEnemyExtra(*m_registry);
    if (config::show_bounding_box) drawLivingBB(*m_registry);
    if (config::show_enemy_texture) drawEnemyTexture(*m_registry, frame / config::enemy_walking_animation_fps);
    if (config::show_attacks) drawAttacks(*m_registry);
    if (config::draw_level_collisions) drawLevelCollisions();

    drawProjectiles(*m_registry);
    drawTooltips(*m_registry);
}
