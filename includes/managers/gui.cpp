//
// Created by ginko on 04/03/24.
//

#include "gui.h"

#include <ranges>
#include <ai/strategies.h>
#include "attributes.h"
#include "parameters.h"
#include "components.h"
#include "constants.h"
#include "config.h"
#include "enemyType.h"
#include "game.h"
#include "items.h"

ImGuiIO *Gui::m_io;

void imguiSplineEditor(entt::registry &registry, const Camera2D &camera) {
    static std::array<Vector2, 4> points{};
    static std::array<bool, 4> pointsCreated = {false};
    static std::array<bool, 4> pointsMoving = {false};

    const auto mouse = GetScreenToWorld2D(GetMousePosition(), camera);
    const auto player = registry.view<Player>().front();
    const auto playerPos = registry.get<Position>(player);

    if (!pointsCreated[0] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[0] = true;
        pointsCreated[1] = true;
        points[0] = Vector2Subtract(mouse, playerPos);
        points[1] = Vector2Add(points[0], {20, 20});
    }
    else if (!pointsCreated[3] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[2] = true;
        pointsCreated[3] = true;
        points[3] = Vector2Subtract(mouse, playerPos);
        points[2] = Vector2Add(points[3], {20, 20});
    }

    for (int i{0}; i < 4; i++) {
        if (CheckCollisionPointCircle(Vector2Subtract(mouse, playerPos), points[i], 5.0f) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            pointsMoving[i] = true;
            break;
        }
    }
    for (int i{0}; i < 4; i++) {
        if (pointsMoving[i]) {
            points[i] = Vector2Subtract(mouse, playerPos);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                pointsMoving[i] = false;
            }
        }
    }
    ImGui::Begin("Spline editor");


    for (int n{0}; n < 4; n++) {
        ImGui::PushID(n);
        ImGui::Text("Point %i: %f x, %f y", n, points[n].x, points[n].y);
        ImGui::PopID();
    }

    static entt::entity entity{registry.create()};
    if (std::views::all(pointsCreated))
        registry.emplace_or_replace<LocalSpline>(entity, points);
    ImGui::End();
}

// Function to render and interact with the table
void imguiEnemyTypesEditor() {
    ImGui::Begin("Enemy Types Editor");

    // Define table columns
    static const char *columnHeaders[] = {
        "Name", "Grade", "Radius", "Speed", "Attack Speed", "Damage",
        "Attack Range", "Attack Spread", "Color", "Health", "Experience",
        "Attributes Path", "Texture Path"
    };

    static EnemyDataFile dataFile{};
    if (dataFile.headers.empty())
        dataFile.loadCSV(dataFile.path);
    static auto enemyList = dataFile.enemyStats;

    if (ImGui::BeginTable("EnemyTable", IM_ARRAYSIZE(columnHeaders),
                          ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        // Render table headers
        for (const char *header: columnHeaders) {
            ImGui::TableSetupColumn(header);
        }
        ImGui::TableHeadersRow();

        // Render rows for each enemy in the list
        for (size_t i = 0; i < enemyList.size(); ++i) {
            EnemyType &enemy = enemyList[i];
            ImGui::TableNextRow();

            char *name = enemy.name.data();
            // Editable fields for each column
            ImGui::TableNextColumn();
            ImGui::InputText(("##Name" + std::to_string(i)).c_str(), name, 80);
            ImGui::TableNextColumn();
            ImGui::InputInt(("##Grade" + std::to_string(i)).c_str(), &enemy.grade);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##Radius" + std::to_string(i)).c_str(), &enemy.radius);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##Speed" + std::to_string(i)).c_str(), &enemy.speed);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##AttackSpeed" + std::to_string(i)).c_str(), &enemy.attackSpeed);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##Damage" + std::to_string(i)).c_str(), &enemy.damage);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##AttackRange" + std::to_string(i)).c_str(), &enemy.attackRange);
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##AttackSpread" + std::to_string(i)).c_str(), &enemy.attackSpread);
            ImGui::TableNextColumn();
            if (ImGui::ColorEdit4(("##Color" + std::to_string(i)).c_str(), (float *) &enemy.color)) {
                // Color edit handler
            }
            ImGui::TableNextColumn();
            ImGui::InputFloat(("##Health" + std::to_string(i)).c_str(), &enemy.health);
            ImGui::TableNextColumn();
            ImGui::InputInt(("##Experience" + std::to_string(i)).c_str(), &enemy.experience);
            // ImGui::TableNextColumn(); ImGui::InputText(("##AttributesPath" + std::to_string(i)).c_str(), &enemy.attributesPath);
            // ImGui::TableNextColumn(); ImGui::InputText(("##TexturePath" + std::to_string(i)).c_str(), &enemy.texturePath);
        }

        ImGui::EndTable();
    }

    // Add a button for adding a new EnemyType
    if (ImGui::Button("Add New Enemy")) {
        // TODO save
        enemyList.push_back(EnemyType{
            "NewEnemy", 1, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            Color(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 10, "", ""
        });
    }
    ImGui::End();
}

void imguiLevel() {
    ImGui::Begin("Level");
    if (ImGui::InputInt("Select Level", &Game::Level)) {
        Game::SetOutcome(LevelOutcome::RESTART);
        Game::ExitLevel();
    }

    ImGui::End();
}


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

void imguiLocalSpace(entt::registry &registry, const Camera2D &camera) {
    auto player = registry.view<Player>().front();
    auto body = registry.get<b2BodyId>(player);
    auto [pos, rot] = b2Body_GetTransform(body);
    const auto [mouseX, mouseY] = GetScreenToWorld2D(GetMousePosition(), camera);
    static auto click = Vector2(0, 0);
    static auto clickTransform = b2Transform{pos, rot};
    static auto clickRad = 0.0f;
    static auto clickDeg = 0.0f;
    const auto radians = atan2(mouseY - pos.y, mouseX - pos.x);
    const auto degrees = radians * RAD2DEG;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        click = {mouseX, mouseY};
        clickTransform = {pos, rot};
        clickRad = radians;
        clickDeg = degrees;
    }

    ImGui::Begin("LocalSpace");
    ImGui::Text("Mouse Local Pos: %f, %f", mouseX - pos.x, mouseY - pos.y);
    ImGui::Text("Mouse Local Rot: %f, %f", rot.c, rot.s);
    ImGui::Text("Mouse Local Rot: %f degrees", degrees);
    ImGui::Text("Mouse Local Rot: %f radians", radians);
    ImGui::Text("Last click: %f x, %f y,\n %f c %f s \n %f deg %f rad",
                click.x - clickTransform.p.x,
                click.y - clickTransform.p.y,
                clickTransform.q.c,
                clickTransform.q.s,
                clickDeg,
                clickRad);
    ImGui::End();
}


void imguiEnemyAttr(entt::registry &registry) {
    ImGui::Begin("Enemy attributes");
    ImGui::Checkbox("show_astar_path", Config::GetBoolPtr("show_astar_path"));
    ImGui::Checkbox("show_enemy_fov", Config::GetBoolPtr("show_enemy_fov"));
    ImGui::Checkbox("show_enemy_texture", Config::GetBoolPtr("show_enemy_texture"));
    ImGui::SeparatorText("Selected Enemy attributes");

    auto view = registry.view<Enemy, Selected, Path, ID, ColorBB, Spread, Speed, Health, Radius, AttackTimer,
        AttackSpeed, Damage, AttackRange, Position, LookAngle,
        Strategy::Strategy>();
    for (auto [entity, path, id, colorbb, spread, speed, health, radius,
             timelastattack, attackspeed, damage,
             attackrange, position, lookAngle, strategy]: view.each()) {
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("lookAngle", &lookAngle.value, -360, 360, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, Const::mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, Const::mapHeight, "%.3f", 0);
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
        ImGui::SliderFloat("position x", &position.x, 0, Const::mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, Const::mapHeight, "%.3f", 0);
    }
    ImGui::End();
}


void imguiAttributesMultipliers() {
    ImGui::Begin("Attribute multipliers");
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); };
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::MultiplierOriginal(subattr) ==
                        Params::Multiplier(subattr);
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::Multiplier(subattr) = Params::MultiplierOriginal(subattr);
        };
        if (disableR) ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::DragFloat(AttributeConstants::subAttributeString[subattr],
                         &Params::Multiplier(subattr),
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
    ImGui::End();
}


void imguiSubAttributesStartValues() {
    ImGui::Begin("Attribute startValues");
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); };
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::StartValue(subattr) ==
                        Params::StartValueOriginal(subattr);
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::StartValue(subattr) = Params::StartValueOriginal(subattr);
        };
        if (disableR) ImGui::EndDisabled();
        ImGui::SameLine();
        ImGui::DragFloat(AttributeConstants::subAttributeString[subattr],
                         &Params::StartValue(subattr),
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
    static bool show_enemy_types_window = false;
    static bool show_local_space_window = false;
    static bool show_level_window = false;
    static auto inEditor = Config::GetBoolPtr("in_editor");
    ImGui::Begin("Main");
    ImGui::Checkbox("Player Window", &show_player_window);
    if (show_player_window)
        imguiPlayerAttr(registry);


    ImGui::Checkbox("Spline Window", Config::GetBoolPtr("show_spline_ui"));
    if (Config::GetBool("show_spline_ui")) {
        *inEditor = true;
        imguiSplineEditor(registry, camera);
    }

    ImGui::Checkbox("Level Window", &show_level_window);
    if (show_level_window)
        imguiLevel();

    ImGui::Checkbox("Cursor Window", &show_cursor_window);
    if (show_cursor_window)
        imguiCursor(camera);

    ImGui::Checkbox("Local Space Window", &show_local_space_window);
    if (show_local_space_window)
        imguiLocalSpace(registry, camera);


    ImGui::Checkbox("Enemy Window", Config::GetBoolPtr("show_enemy_window"));
    if (Config::GetBool("show_enemy_window"))
        imguiEnemyAttr(registry);

    ImGui::Checkbox("Enemy Types Window", &show_enemy_types_window);
    if (show_enemy_types_window)
        imguiEnemyTypesEditor();

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


void Gui::Instantiate() {
    ImGui::CreateContext();
    m_io = &ImGui::GetIO();
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplRaylib_Init();

    m_io->Fonts->AddFontDefault();
    ImGui_ImplRaylib_BuildFontAtlas();
}

void Gui::Update(entt::registry &registry, const Camera2D &camera) {
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    imguiWindowMain(registry, *m_io, camera);
    ImGui::Render();
}

void Gui::Draw() {
    ImGui_ImplRaylib_RenderDrawData(ImGui::GetDrawData());
}

bool Gui::WantMouse() {
    return m_io->WantCaptureMouse;
}

bool Gui::WantKeyboard() {
    return m_io->WantCaptureKeyboard;
}

Gui::~Gui() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}
