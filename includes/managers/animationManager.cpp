//
// Created by ginko on 19/04/24.
//

#include "pch.h"
#include "animationManager.h"
#include "../constants.h"

namespace fs = std::filesystem;

std::unordered_map<size_t, TextureAnimation> AnimationManager::resources;
std::hash<std::string> AnimationManager::hasher;
AnimationManager *AnimationManager::instance;

Texture2D &AnimationManager::getTexture(const std::string &key, unsigned int frame) {
    TextureAnimation &anim = resources.at(hasher(key));
    return anim.textures[frame % anim.frameCount];
}

void AnimationManager::LoadFromDirectory(const std::string &directory) {
    TextureAnimation anim;
    std::set<fs::path> sorted_by_name;

    for (auto &entry : fs::directory_iterator(getAssetPath(directory))) {
        sorted_by_name.insert(entry.path());
    }

    for (auto &filename : sorted_by_name) {
        anim.textures.push_back(LoadTexture(reinterpret_cast<const char *>(filename.c_str())));
        anim.frameCount++;
    }

    resources.insert({hasher(directory), anim});
}


AnimationManager &AnimationManager::Instance() { return *instance; }


AttackEffect::AttackEffect(float mDuration, Vector2 mCenter, const float &mRadius,
                           float mStartAngle, float mEndangle, const Color &mColor) : m_duration(mDuration),
                                                                                      m_center(mCenter),
                                                                                      m_radius(mRadius),
                                                                                      m_color(mColor),
                                                                                      m_startAngle(mStartAngle),
                                                                                      m_endAngle(mEndangle) {
    m_timer.Reset();
}

void AttackEffect::Draw() const {
    DrawCircleSector({m_center.x, m_center.y}, m_radius, m_startAngle, m_endAngle, 2,
                     ColorAlpha(m_color, 1 - m_timer.ElapsedMillis() / m_duration));
}

bool AttackEffect::Expired() const { return m_timer.ElapsedMillis() > m_duration; }
