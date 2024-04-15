//
// Created by ginko on 04/03/24.
//

#ifndef GUI_GUI_H
#define GUI_GUI_H


#include <entt/entity/registry.hpp>
#include "imgui/imgui.h"

ImGuiIO InitGui();

void UpdateGui(entt::registry &registry, ImGuiIO &io);

void DrawGui();

void CloseGui();

void imguiPlayerAttr();


#endif //GUI_GUI_H
