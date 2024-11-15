//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#include <entt/entity/registry.hpp>
#include <imgui.h>

#include "raylib.h"

class Gui {
    static ImGuiIO *m_io;
    static entt::registry *m_registry;

public:
    static void Instantiate(entt::registry &registry);

    static void Update(const Camera2D &camera);

    static void Draw();

    ~Gui();
};

ImGuiIO InitGui();


#endif //GUI_MANAGER_H
