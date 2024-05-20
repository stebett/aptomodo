//
// Created by ginko on 04/03/24.
//

#include "gui.h"
#include <imgui_impl_raylib.h>
#include "components.h"
#include "constants.h"
#include <entt/entity/registry.hpp>
#include "config.h"
#include "attributes.h"


void imguiEnemyAttr(entt::registry &registry) { //TODO: make this work on selected enemy
    auto view = registry.view<Enemy, Path, ID, ColorBB, Spread, Speed, Health, Radius, PhysicalResistance, MagicalResistance, Stamina, AttackTimer, AttackSpeed, Damage, AttackRange, Pushback, Position>();
    for (auto [entity, path, id, colorbb, spread, speed, health, radius, physicalresistance, magicalresistance, stamina, timelastattack, attackspeed, damage, attackrange, pushback, position
        ]: view.each()) {
        // Only plot this for one enemy
        static int enemyID = id;
        if (id != enemyID) break;
        ImGui::SliderFloat("spread", &spread.value, 0, 360, "%.3f", 0);
        ImGui::SliderFloat("speed.value", &speed.value, 0, 15, "%.3f", 0);
        ImGui::SliderFloat("speed.actual", &speed.actual, 0, 15, "%.3f", 0);
        ImGui::SliderFloat("speed.max", &speed.max, 0, 15, "%.3f", 0);
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
        Vector2 currentPath = path.getCurrent();
        ImGui::SliderFloat("Target x", &currentPath.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("Target y ", &currentPath.y, 0, mapHeight, "%.3f", 0);
    }
}


void imguiPlayerAttr(entt::registry &registry) {
    auto view = registry.view<Player, ColorBB, Spread, Speed, Health, Radius, PhysicalResistance, MagicalResistance, Stamina, AttackTimer, AttackSpeed, Damage, AttackRange, Pushback, Position>();
    for (auto [entity, colorbb, spread, speed, health, radius, physicalresistance, magicalresistance, stamina, timelastattack, attackspeed, damage, attackrange, pushback, position
        ]: view.each()) {
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, mapHeight, "%.3f", 0);

    }
}

void imguiItems(entt::registry &registry) {
    auto view = registry.view<OnPlayer, Item, Name>();
    for (auto [entity, name]: view.each()) {
        if (ImGui::Button(name.value.c_str())) {
            for (auto [player, position]: registry.view<Player, Position>().each()) {
                registry.remove<OnPlayer>(entity);
                registry.emplace<Position>(entity, Vector2Add(position, {10, 10}));
            }
        }
    }
}


void imguiAttributesMultipliers() {
    ImGui::Begin("Attribute multipliers");
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        ImGui::SliderFloat(AttributeConstants::subAttributeString[subattr], &config::attrMultipliers[subattr], 0, 30,
                           "%.3f",
                           0);
        ImGui::PopID();
    }
    ImGui::End();
}

void imguiAttributes(entt::registry &registry) {
    ImGui::Begin("Attributes");

    int n = {0};
    int m = {0};
    bool disableUp{false};
    bool disableDown{false};
    auto view = registry.view<Player, Experience, Attributes>();
    for (auto [entity, experience, attributes]: view.each()) {
        ImGui::Text("Level %d", attributes.getLevel());
        ImGui::SameLine();
        ImGui::ProgressBar(static_cast<float>(experience - attributes.expForCurrentLevel()) /
                           static_cast<float>(attributes.expToNextLevel() - attributes.expForCurrentLevel()));
        ImGui::Text("Exp %d", experience);

        ImGui::SeparatorText(std::format("Attributes - free points: {}", attributes.freeAttrPoints()).c_str());
        for (auto attr: AttributeConstants::attributeVec) {
            ImGui::PushID(n);
            disableUp = attributes.outOfAttrPoints();
            disableDown = attributes.atMinAttrPoints(attr);
            if (disableUp) ImGui::BeginDisabled(true);
            if (ImGui::ArrowButton("##up", ImGuiDir_Up)) attributes.increase(attr);
            if (disableUp) ImGui::EndDisabled();
            ImGui::SameLine();
            if (disableDown) ImGui::BeginDisabled(true);
            if (ImGui::ArrowButton("##down", ImGuiDir_Down)) attributes.decrease(attr);
            if (disableDown) ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::Text("%d", attributes.get(attr));
            ImGui::SameLine();
            ImGui::Text("%s", AttributeConstants::attributeString[attr]);
            ImGui::SeparatorText(
                    std::format("Sub-Attributes - free points: {}", attributes.freeSubAttrPoints(attr)).c_str());

            for (auto subattr: AttributeConstants::subAttrByAttr[attr]) {
                ImGui::PushID(m);
                disableUp = attributes.outOfSubAttrPoints(subattr);
                disableDown = attributes.atMinSubAttrPoints(subattr);
                ImGui::PushButtonRepeat(true);
                if (disableUp) ImGui::BeginDisabled(true);
                if (ImGui::ArrowButton("##up", ImGuiDir_Up)) attributes.increase(subattr);
                if (disableUp) ImGui::EndDisabled();
                ImGui::SameLine();
                if (disableDown) ImGui::BeginDisabled(true);
                if (ImGui::ArrowButton("##down", ImGuiDir_Down)) attributes.decrease(subattr);
                if (disableDown) ImGui::EndDisabled();
                ImGui::PopButtonRepeat();
                ImGui::SameLine();
                ImGui::Text("%d", attributes.get(subattr));
                ImGui::SameLine();
                ImGui::Text("%s: ", AttributeConstants::subAttributeString[subattr]);
                ImGui::SameLine();
                ImGui::Text("%.1f", attributes.getMultiplied(subattr));
                ImGui::PopID();
                m++;
            }
            ImGui::PopID();
            n++;
        }
    }
    static bool show_multipliers = false;
    ImGui::Checkbox("show_multipliers", &show_multipliers);
    if (show_multipliers)
        imguiAttributesMultipliers();

    static bool show_items = false;
    ImGui::Checkbox("show_items", &show_items);
    if (show_items)
        imguiItems(registry);

    ImGui::End();
}


void imguiConfig() {
    ImGui::Checkbox("show_astar_path", &config::show_astar_path);
    ImGui::Checkbox("show_enemy_fov", &config::show_enemy_fov);
    ImGui::Checkbox("show_bounding_box", &config::show_bounding_box);
    ImGui::Checkbox("show_enemy_texture", &config::show_enemy_texture);
    ImGui::Checkbox("show_attacks", &config::show_attacks);
    ImGui::SliderInt("FPS", &config::fps, 0, 120);
    ImGui::SliderInt("enemy_walking_animation_fps", &config::enemy_walking_animation_fps, 1, 120);
    ImGui::SliderInt("strategy", &config::strategy, 0, 2);
}

void imguiWindowMain(entt::registry &registry, ImGuiIO io) {
    static bool show_demo_window = false;
    static bool show_player_window = false;
    static bool show_config_window = false;
    static bool show_enemy_window = false;

    ImGui::Begin("Main");


    ImGui::Checkbox("Demo Window", &show_demo_window);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Checkbox("Player Window", &show_player_window);
    if (show_player_window)
        imguiPlayerAttr(registry);

    ImGui::Checkbox("Enemy Window", &show_enemy_window);
    if (show_enemy_window)
        imguiEnemyAttr(registry);

    ImGui::Checkbox("Config Window", &show_config_window);
    if (show_config_window)
        imguiConfig();

    ImGui::Checkbox("Attributes Window", &config::show_attr_window);
    if (config::show_attr_window)
        imguiAttributes(registry);


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