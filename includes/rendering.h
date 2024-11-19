//
// Created by ginko on 02/03/24.
//
// Credits to https://github.com/tupini07/raylib-cpp-cmake-template

#ifndef APTOMODO_RENDERING_H
#define APTOMODO_RENDERING_H


#include <raylib.h>
#include "components.h"


void updateCamera(Camera2D &camera, Position &playerPosition);
void switchCamera(Camera2D &camera, Position &playerPosition);
//
//class GameScene {
//
//    int current_level;
//
//    entt::registry& m_registry;
//
//    const ldtk::Project *ldtkProject{};
//    const ldtk::World *ldtkWorld{};
//    ldtk::Level *currentLdtkLevel{};
//    std::unordered_map<std::string
//
//    Texture2D currentTilesetTexture;
//    Texture2D renderedLevelTexture;
//
//public:
//    Map grid;
//
//    GameScene(entt::registry &registry);
//
//    ~GameScene();
//
//    void setLevel(entt::registry &registry, int level);
//    void draw();
////    std::string getTexturePath(const std::string& tileset);
////    Animation::Map getAnimationMap(const std::string& ldtkEnum);
//};
//
//void draw(entt::registry &registry, GameScene *scene, unsigned int frame);
//
//
//class AttackEffect {
//    Timer m_timer;
//    Position &m_center;
//    Color m_color;
//    float m_radius;
//    float m_duration;
//    float m_startAngle;
//    float m_endAngle;
//
//
//public:
//    AttackEffect(float mDuration, Position &mCenter, const float &mRadius,
//                 float mStartAngle, float mEndangle, const Color &mColor) : m_duration(mDuration),
//                                                                            m_center(mCenter),
//                                                                            m_radius(mRadius),
//                                                                            m_color(mColor),
//                                                                            m_startAngle(mStartAngle),
//                                                                            m_endAngle(mEndangle) {
//        m_timer.Reset();
//    }
//
//    void Draw() const {
//        DrawCircleSector({m_center.x, m_center.y}, m_radius, m_startAngle, m_endAngle, 2,
//                         ColorAlpha(m_color, 1 - m_timer.ElapsedMillis() / m_duration ));
//    }
//
//    [[nodiscard]] bool Expired() const {return m_timer.ElapsedMillis() > m_duration;}
//};


#endif //APTOMODO_RENDERING_H
