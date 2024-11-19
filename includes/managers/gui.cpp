//
// Created by ginko on 04/03/24.
//

#include "pch.h"
#include "gui.h"
#include "attributes.h"
#include "parameters.h"
#include "components.h"
#include "constants.h"
#include "config.h"
#include "items.h"

ImGuiIO *Gui::m_io;
entt::registry *Gui::m_registry;

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
    ImGui::Begin("Selected Enemy attributes");

    auto view = registry.view<Enemy, Selected, Path, ID, ColorBB, Spread, Speed, Health, Radius, PhysicalResistance,
        MagicalResistance, Stamina, AttackTimer, AttackSpeed, Damage, AttackRange, Pushback, Position>();
    for (auto [entity, path, id, colorbb, spread, speed, health, radius, physicalresistance, magicalresistance, stamina,
             timelastattack, attackspeed, damage, attackrange, pushback, position
         ]: view.each()) {
        //        // Only plot this for one enemy
        //        static int enemyID = id;
        //        if (id != enemyID) break;
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, mapHeight, "%.3f", 0);
        ImGui::SliderFloat("enemySightRange", &config::enemySightRange, 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemySightRangeChasing", &config::enemySightRangeChasing, 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemyHearRange", &config::enemyHearRange, 0, 800, "%.3f", 0);
        ImGui::SliderFloat("enemyHearRangeChasing", &config::enemyHearRangeChasing, 0, 800, "%.3f", 0);
    }
    ImGui::End();
}


void imguiPlayerAttr(entt::registry &registry) {
    ImGui::Begin("Player values");

    auto view = registry.view<Player, ColorBB, Spread, Speed, Health, Radius, PhysicalResistance, MagicalResistance,
        Stamina, AttackTimer, AttackSpeed, Damage, AttackRange, Pushback, Position>();
    for (auto [entity, colorbb, spread, speed, health, radius, physicalresistance, magicalresistance, stamina,
             timelastattack, attackspeed, damage, attackrange, pushback, position
         ]: view.each()) {
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
    ImGui::Checkbox("show_bounding_box", &config::show_bounding_box);
    ImGui::Checkbox("show_attacks", &config::show_attacks);
    ImGui::Checkbox("free_camera", &config::free_camera);
    ImGui::Checkbox("draw_level_collisions", &config::draw_level_collisions);
    ImGui::SliderInt("FPS", &config::fps, 0, 120);
    ImGui::SliderInt("strategy", &config::strategy, 0, 2);
    ImGui::Checkbox("show_astar_path", &config::show_astar_path);
    ImGui::Checkbox("show_enemy_fov", &config::show_enemy_fov);
    ImGui::Checkbox("show_enemy_texture", &config::show_enemy_texture);
    ImGui::SliderInt("enemy_walking_animation_fps", &config::enemy_walking_animation_fps, 1, 120);
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


    ImGui::Checkbox("Enemy Window", &config::show_enemy_window);
    if (config::show_enemy_window)
        // todo auto open when enemy selected
        imguiEnemyAttr(registry);

    ImGui::Checkbox("Config Window", &show_config_window);
    if (show_config_window)
        imguiConfig();

    ImGui::Checkbox("Attributes Window", &config::show_attr_window);
    if (config::show_attr_window)
        imguiAttributes(registry);

    ImGui::Checkbox("Inventory Window", &config::show_inv_window);
    if (config::show_inv_window)
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


void Gui::Instantiate(entt::registry &registry) {
    m_registry = &registry;
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    m_io->Fonts->AddFontDefault();
    ImGui_ImplRaylib_BuildFontAtlas();
}

void Gui::Update(const Camera2D &camera) {
    ImGui_ImplRaylib_ProcessEvents();

    // Start the Dear ImGui frame
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(*m_registry, *m_io, camera);

    // Rendering
    ImGui::Render();
}

void Gui::Draw() {
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}

Gui::~Gui() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}
