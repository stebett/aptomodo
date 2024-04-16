//
// Created by ginko on 04/03/24.
//

#include "gui.h"
#include "rlImGui/imgui_impl_raylib.h"
#include "components.h"
#include "constants.h"
#include <entt/entity/registry.hpp>
#include "config.h"


void imguiPlayerAttr(entt::registry &registry) {
    auto view = registry.view<Player, ColorBB, Spread, Speed, Health, Radius, PhysicalResistance, MagicalResistance, Stamina, AttackTimer, AttackSpeed, Damage, AttackRange, Pushback, Position>();
    for (auto [entity, colorbb, spread, speed, health, radius, physicalresistance, magicalresistance, stamina, timelastattack, attackspeed, damage, attackrange, pushback, position
        ]: view.each()) {

        ImGui::SliderFloat("spread", &spread.value, 0, 360, "%.3f", 0);
        ImGui::SliderFloat("speed", &speed.value, 0, 15, "%.3f", 0);
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("physicalresistance", &physicalresistance.value, 0, 30, "%.3f", 0);
        ImGui::SliderFloat("magicalresistance", &magicalresistance.value, 0, 30, "%.3f", 0);
        ImGui::SliderFloat("stamina", &stamina.value, 0, 30, "%.3f", 0);
        ImGui::SliderFloat("attackspeed", &attackspeed.value, 0, 5, "%.3f", 0);
        ImGui::SliderFloat("damage", &damage.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("attackrange", &attackrange.value, 1, 150, "%.3f", 0);
        ImGui::SliderFloat("pushback", &pushback.value, 0, 150, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, mapHeight, "%.3f", 0);

    }
}

void imguiConfig() {
    ImGui::Checkbox("Astar path", &config::show_astar_path);
}

void imguiWindowMain(entt::registry &registry, ImGuiIO io) {
    static bool show_demo_window = false;
    static bool show_player_window = false;
    static bool show_config_window = false;

    ImGui::Begin("Main");


    ImGui::Checkbox("Demo Window", &show_demo_window);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Checkbox("Player Window", &show_player_window);
    if (show_player_window)
        imguiPlayerAttr(registry);

    ImGui::Checkbox("Config Window", &show_config_window);
    if (show_config_window)
        imguiConfig();

//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}


ImGuiIO InitGui() {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    io.Fonts->AddFontDefault();
    Imgui_ImplRaylib_BuildFontAtlas();

    return io;
}


void UpdateGui(entt::registry &registry, ImGuiIO &io) {
    ImGui_ImplRaylib_ProcessEvents();

// Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(registry, io);

// Rendering
    ImGui::Render();
}

void DrawGui() {
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());

}

void CloseGui() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}