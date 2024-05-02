//
// Created by ginko on 17/04/24.
//

#include "audioManager.h"
#include "renderingManager.h"
#include "../components.h"
#include "animationManager.h"
#include "../config.h"



RenderingManager *RenderingManager::instance;
entt::registry *RenderingManager::m_registry;

RenderingManager &RenderingManager::Instance() {
    return *instance;
}


RenderingManager::RenderingManager(entt::registry &registry)  {
    m_registry = &registry;
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
    auto livingView = registry.view<Living, Radius, Position, ColorBB>();
    for (auto [entity, radius, position, color]: livingView.each()) {
        DrawCircle(position.x, position.y, radius, ColorAlpha(color, 0.3));
    }
}


void draw(entt::registry &registry, unsigned int frame) {
    drawLivingBB(registry);
}
void RenderingManager::Draw(unsigned int frame) {
    if (config::show_bounding_box) drawLivingBB(*m_registry);
    if (config::show_enemy_texture) drawEnemyTexture(*m_registry, frame / config::enemy_walking_animation_fps);
    if (config::show_attacks) drawAttacks(*m_registry);

}
