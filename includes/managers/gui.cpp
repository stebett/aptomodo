//
// Created by ginko on 04/03/24.
//

#include "pch.h"
#include "gui.h"

#include <ai/strategies.h>

#include "attributes.h"
#include "parameters.h"
#include "components.h"
#include "constants.h"
#include "config.h"
#include "items.h"

ImGuiIO *GuiManager::m_io;

void imguiInstructions() {
    ImGui::Begin("Instructions");
    ImGui::SeparatorText("Hotkeys");

    ImGui::Text("Q per chiudere");
    ImGui::Text("R per restartare");
    ImGui::Text("P per pausare");
    ImGui::Text("I per togglare inventario");
    ImGui::Text("O per togglare attributi");
    ImGui::Text("LClick per attaccare");
    ImGui::Text("RClick per selezionare");
    ImGui::End();
}

void imguiCursor(const Camera2D &camera) {
    ImGui::Begin("Cursor");
    ImGui::Text("Screen Position: %f, %f", GetMousePosition().x, GetMousePosition().y);
    ImGui::Text("World Position: %f, %f", GetScreenToWorld2D(GetMousePosition(), camera).x,
                GetScreenToWorld2D(GetMousePosition(), camera).y);
    ImGui::Text("Camera Zoom: %f", camera.zoom);
    ImGui::Text("Camera target: %f, %f", camera.target.x, camera.target.y);
    ImGui::Text("Camera offset: %f, %f", camera.offset.x, camera.offset.y);
    ImGui::End();
}

void imguiEnemyAttr(entt::registry &registry) {
    ImGui::Begin("Enemy attributes");
    ImGui::SliderInt("strategy", Config::GetIntPtr("strategy"), 0, 2);
    ImGui::Checkbox("show_astar_path", Config::GetBoolPtr("show_astar_path"));
    ImGui::Checkbox("show_enemy_fov", Config::GetBoolPtr("show_enemy_fov"));
    ImGui::Checkbox("show_enemy_texture", Config::GetBoolPtr("show_enemy_texture"));
    ImGui::SeparatorText("Selected Enemy attributes");

    ImGui::SliderInt("enemy_walking_animation_fps", Config::GetIntPtr("enemy_walking_animation_fps"), 1, 120);
    auto view = registry.view<Enemy, Selected, Path, ID, ColorBB, Spread, Speed, Health, Radius, AttackTimer,
        AttackSpeed, Damage, AttackRange, Pushback, Position, LookAngle,
        Strategy::Strategy>();
    for (auto [entity, path, id, colorbb, spread, speed, health, radius,
             timelastattack, attackspeed, damage,
             attackrange, pushback, position, lookAngle, strategy]: view.each()) {
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("lookAngle", &lookAngle.value, -360, 360, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, mapHeight, "%.3f", 0);
        ImGui::SliderFloat("enemySightRange", Config::GetFloatPtr("enemySightRange"), 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemySightRangeChasing", Config::GetFloatPtr("enemySightRangeChasing"), 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemyHearRange", Config::GetFloatPtr("enemyHearRange"), 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemyHearRangeChasing", Config::GetFloatPtr("enemyHearRangeChasing"), 0, 800, "%.3f", 0);

        ImGui::SeparatorText("Behavior Tree");
        std::vector<std::pair<const char *, Status> > result;
        collectNodeStatus(strategy.behavior->getRoot(), result);

        int n = 0;
        for (auto [name, status]: result) {
            ImGui::PushID(n);
            ImVec4 color = {};
            switch (status) {
                case SUCCESS: {
                    color = ImVec4(0, 255, 0, 255);
                    break;
                }
                case FAILURE: {
                    color = ImVec4(255, 0, 0, 255);
                    break;
                }
                case INVALID: {
                    color = ImVec4(0, 40, 200, 255);
                    break;
                }
                default:
                    color = ImVec4(255, 255, 255, 255);
            }
            ImGui::TextColored(color, name);
            ImGui::PopID();
            n++;
        }
    }
    ImGui::End();
}


void imguiPlayerAttr(entt::registry &registry) {
    ImGui::Begin("Player values");

    auto view = registry.view<Player, ColorBB, Spread, Speed, Health, Radius, AttackTimer, AttackSpeed, Damage,
        AttackRange, Pushback, Position>();
    for (auto [entity, colorbb, spread, speed, health, radius, timelastattack,
             attackspeed, damage, attackrange,pushback, position]: view.each()) {
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, mapHeight, "%.3f", 0);
    }
    ImGui::End();
}


void imguiAttributesMultipliers() {
    ImGui::Begin("Attribute multipliers");
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); };
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::attributes.subAttrMultipliers[subattr] ==
                        Params::attributesOriginal.subAttrMultipliers[subattr];
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::attributes.subAttrMultipliers[subattr] = Params::attributesOriginal.subAttrMultipliers[subattr];
        };
        if (disableR) ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::DragFloat(AttributeConstants::subAttributeString[subattr],
                         &Params::attributes.subAttrMultipliers[subattr],
                         0.1f, 0, 30);
        ImGui::PopID();
        n++;
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
    ImGui::End();
}

void imguiInventory(entt::registry &registry) {
    ImGui::Begin("Inventory");

    auto view = registry.view<OnPlayer, Item, Name, AttributeConstants::Modifier>();
    for (auto [entity, name, modifier]: view.each()) {
        ImGui::Text("%s", name.value.c_str());
        ImGui::SameLine();
        if (ImGui::Button("Drop")) {
            for (auto [player, position]: registry.view<Player, Position>().each()) {
                registry.remove<OnPlayer>(entity);
                registry.emplace<Position>(entity, Vector2Add(position, {10, 10}));
            }
        }
        ImGui::Text("Modifier: %s", AttributeConstants::attributeString[modifier.name]);
        ImGui::Text("Operation: %s", AttributeConstants::operatorString[static_cast<int>(modifier.operation)]);
        ImGui::Text("Value: %f", modifier.value);
        ImGui::Separator();
    }

    ImGui::End();
}


void imguiConfig() {
    ImGui::Begin("Config");
    ImGui::SeparatorText("Bools");
    int n{0};
    for (const auto &[name, index]: Config::indexDictBools) {
        ImGui::PushID(n);
        ImGui::Checkbox(name.c_str(), Config::GetBoolPtr(name));
        ImGui::PopID();
        n++;
    }
    ImGui::SeparatorText("Ints");
    for (const auto &[name, index]: Config::indexDictInts) {
        ImGui::PushID(n);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
        ImGui::InputInt(name.c_str(), Config::GetIntPtr(name));
        ImGui::PopID();
        n++;
    }
    ImGui::SeparatorText("Floats");
    for (const auto &[name, index]: Config::indexDictFloats) {
        ImGui::PushID(n);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.25f);
        ImGui::InputFloat(name.c_str(), Config::GetFloatPtr(name), 0, 0, "%.1f");
        ImGui::PopID();
        n++;
    }
    ImGui::SeparatorText("Extra");
    ImGui::Checkbox("show_bounding_box", Config::GetBoolPtr("show_bounding_box"));
    ImGui::Checkbox("show_attacks", Config::GetBoolPtr("show_attacks"));
    ImGui::Checkbox("free_camera", Config::GetBoolPtr("free_camera"));
    ImGui::Checkbox("draw_level_collisions", Config::GetBoolPtr("draw_level_collisions"));
    ImGui::End();
}


void imguiSubAttributesStartValues() {
    ImGui::Begin("Attribute startValues");
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); };
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::attributes.subAttrAtStart[subattr] ==
                        Params::attributesOriginal.subAttrAtStart[subattr];
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::attributes.subAttrAtStart[subattr] = Params::attributesOriginal.subAttrAtStart[subattr];
        };
        if (disableR) ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::DragFloat(AttributeConstants::subAttributeString[subattr],
                         &Params::attributes.subAttrAtStart[subattr],
                         0.1f, 0, 30);
        ImGui::PopID();
        n++;
    }

    ImGui::End();
}

void imguiWindowMain(entt::registry &registry, ImGuiIO io, const Camera2D &camera) {
    static bool show_config_window = false;
    static bool show_player_window = false;
    static bool show_instructions_window = false;
    static bool show_multipliers = false;
    static bool show_start_values = false;
    static bool show_cursor_window = false;
    ImGui::Begin("Main");


    ImGui::Checkbox("Player Window", &show_player_window);
    if (show_player_window)
        imguiPlayerAttr(registry);

    ImGui::Checkbox("Cursor Window", &show_cursor_window);
    if (show_cursor_window)
        imguiCursor(camera);


    ImGui::Checkbox("Enemy Window", Config::GetBoolPtr("show_enemy_window"));
    if (Config::GetBool("show_enemy_window"))
        // todo auto open when enemy selected
        imguiEnemyAttr(registry);

    ImGui::Checkbox("Config Window", &show_config_window);
    if (show_config_window)
        imguiConfig();

    ImGui::Checkbox("Attributes Window", Config::GetBoolPtr("show_attr_window"));
    if (Config::GetBool("show_attr_window"))
        imguiAttributes(registry);

    ImGui::Checkbox("Inventory Window", Config::GetBoolPtr("show_inv_window"));
    if (Config::GetBool("show_inv_window"))
        imguiInventory(registry);

    ImGui::Checkbox("Instructions", &show_instructions_window);
    if (show_instructions_window)
        imguiInstructions();

    ImGui::Checkbox("show_multipliers", &show_multipliers);
    if (show_multipliers)
        imguiAttributesMultipliers();

    ImGui::Checkbox("show_start_values", &show_start_values);
    if (show_start_values)
        imguiSubAttributesStartValues();

    //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io.Framerate, m_io.Framerate);
    ImGui::End();
}


void GuiManager::Instantiate() {
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    m_io->Fonts->AddFontDefault();
    ImGui_ImplRaylib_BuildFontAtlas();
}

void GuiManager::Update(entt::registry &registry, const Camera2D &camera) {
    ImGui_ImplRaylib_ProcessEvents();

    // Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(registry, *m_io, camera);

    // Rendering
    ImGui::Render();
}

void GuiManager::Draw() {
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}

GuiManager::~GuiManager() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}
