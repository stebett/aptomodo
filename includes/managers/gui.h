//
// Created by ginko on 04/03/24.
//

#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H


#include <entt/entity/registry.hpp>
#include <imgui.h>
#include "raylib.h"

void imguiWindowMainAI(const Camera2D &camera);
void imguiWindowMain(const Camera2D &camera);

class Gui {
    static ImGuiIO *m_io;

public:
    static void Instantiate();

    static void Update(const Camera2D &camera, const std::function<void(const Camera2D &)>&);

    static void Draw();

    static bool WantMouse();

    static bool WantKeyboard();

    static void Clean();
};

ImGuiIO InitGui();


#endif //GUI_MANAGER_H
