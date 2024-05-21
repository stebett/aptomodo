//
// Created by ginko on 04/03/24.
//

#ifndef GUI_GUI_H
#define GUI_GUI_H


#include "entt/entity/registry.hpp"
#include "imgui.h"

ImGuiIO InitGui();
// TODO Make this a class
void UpdateGui(entt::registry &registry, ImGuiIO &io);

void DrawGui();

void CloseGui();

void imguiPlayerAttr();


#endif //GUI_GUI_H
