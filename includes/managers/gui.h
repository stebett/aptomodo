//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#include <entt/entity/registry.hpp>
#include <imgui.h>
#include "raylib.h"

class GuiManager {
    static ImGuiIO *m_io;

public:
    static void Instantiate();

    static void Update(entt::registry& registry, const Camera2D &camera);

    static void Draw();

    ~GuiManager();
};

ImGuiIO InitGui();


#endif //GUI_MANAGER_H
