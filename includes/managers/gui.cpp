//
// Created by ginko on 04/03/24.
//

#include "gui.h"

#include <ranges>
#include <ai/strategies.h>
#include <math/mathConstants.h>
#include <systems/physics.h>

#include "attacks.h"
#include "attributes.h"
#include "parameters.h"
#include "components.h"
#include "constants.h"
#include "configManager.h"
#include "resources/enemyType.h"
#include "game.h"
#include "items.h"
#include "gui/bezierEditor.h"
#include "assets.h"
#include "factories.h"
#include "audioManager.h"

ImGuiIO *Gui::m_io;

void modalDroppedMp3(const std::filesystem::path &filepath, bool *droppedFile) {
    ImGui::OpenPopup("MP3 file dropped");
    auto path = std::filesystem::path(ROOT_PATH) / std::filesystem::path(AUDIO_PATH) / filepath.filename();
    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("MP3 file dropped", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Vuoi salvare il file?");
        ImGui::Text("Verra' salvato in %s", path.c_str());
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            std::filesystem::copy(filepath, path);
            auto names = Audio::loadCSV();
            auto name = filepath.stem();
            names.emplace_back(name.string());
            Audio::saveCSV(names);
            Assets::LoadAudio(name);
            *droppedFile = false;
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
            *droppedFile = false;
        }
        ImGui::EndPopup();
    }
}

void imguiShowAssets() {
    ImGui::Begin("Assets");
    if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_None)) {
        for (const auto &[id, soundEffect]: Assets::audioResources) {
            ImGui::PushID(id);
            if (ImGui::Button(soundEffect.name.c_str()))
                Game::registry.emplace<Audio::Command>(Game::registry.create(), soundEffect.name);
            ImGui::PopID();
        }
    }
    if (ImGui::CollapsingHeader("Enemies", ImGuiTreeNodeFlags_None)) {
        const auto player = Game::registry.view<Player>().front();
        const auto playerPosition = Game::registry.get<Position>(player);
        const auto enemyList = Assets::GetEnemiesData().enemyStats;
        for (size_t i = 0; i < enemyList.size(); ++i) {
            const EnemyType &enemy = enemyList[i];
            ImGui::PushID(i);
            if (ImGui::Button(enemy.name.c_str()))
                spawnEnemyByName(playerPosition + Vector2{0.0f, 40.0f}, enemy.name);
            ImGui::PopID();
        }
    }
    ImGui::End();
};

auto createBody = [](const entt::entity entity) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    const b2BodyId body = b2CreateBody(Physics::GetWorldID(), &bodyDef);
    Physics::ConnectBodyToEntity(body, entity);
    return body;
};

auto createShape = [](b2BodyId bodyId, b2Polygon box) {
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.isSensor = true;
    shapeDef.filter.maskBits = Physics::Enemy;
    return b2CreatePolygonShape(bodyId, &shapeDef, &box);
};

// Bezier editors for easing parameters
auto bezierEditor = [](const char *label, EasingSpline &easing) {
    static std::unordered_map<std::string, bool> checkboxStates;
    bool &show_window = checkboxStates[label];
    ImGui::Checkbox(label, &show_window);
    auto easing_points = easing.get();
    if (show_window) ImGui::ShowBezierEditor(easing_points.data(), label, &show_window);
    easing.update(easing_points);
};


// Parameter sliders with reset buttons
auto sliderWithReset = [](const char *label, float &value, float min, float max, float resetValue) {
    if (ImGui::Button("R")) value = resetValue;
    ImGui::SameLine();
    ImGui::SliderFloat(label, &value, min, max);
};


void imguiSplineEditor(const Camera2D &camera) {
    // Static variables for spline editor state
    static std::array<b2Vec2, 4> points{};
    static std::array<b2Transform, 4> transforms{};
    static std::array<bool, 4> pointsCreated = {false};
    static std::array<bool, 4> pointsMoving = {false};

    // Get mouse position in world coordinates
    const Math::Vec2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

    // Get player body and its transform
    const auto player = Game::registry.view<Player>().front();
    const auto playerBody = Game::registry.get<b2BodyId>(player);
    const auto playerTransform = b2Body_GetTransform(playerBody);

    // Handle mouse input for creating the first two points
    if (!pointsCreated[0] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[0] = pointsCreated[1] = true;
        points[0] = mouse;
        points[1] = b2Add(points[0], {20, 20});
        transforms[0] = transforms[1] = playerTransform;
    }
        // Handle mouse input for creating the last two points
    else if (!pointsCreated[3] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[2] = pointsCreated[3] = true;
        points[3] = mouse;
        points[2] = b2Add(points[3], {20, 20});
        transforms[2] = transforms[3] = playerTransform;
    }

    // Create and update the local spline
    auto spline = LocalSpline{points, transforms};

    // Check for point movement
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointCircle(spline.getGlobal(playerTransform)[i], mouse, 5.0f) &&
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            pointsMoving[i] = true;
            break;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (pointsMoving[i]) {
            points[i] = mouse;
            transforms[i] = playerTransform;
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                pointsMoving[i] = false;
            }
        }
    }

    // Start ImGui spline editor window
    ImGui::Begin("Spline editor");

    // Store the spline in the registry so it can be rendered
    static entt::entity entity = Game::registry.create();
    Game::registry.emplace_or_replace<LocalSpline>(entity, spline);


    // Spline parameters
    static float duration = 2.0f;
    static float interval = 0.5f;

    // Editor for Transform parameters
    static auto s = Attacks::Transform{spline};
    s.spline = spline;

    ImGui::SeparatorText("Parameters Editor");
    sliderWithReset("Start Radians", s.startRadians, -3.14f, 3.14f, 0.0f);
    sliderWithReset("End Radians", s.endRadians, -3.14f, 3.14f, 0.0f);
    sliderWithReset("Start Dim1", s.startDim1, 0.1f, 30.0f, 5.0f);
    sliderWithReset("Start Dim2", s.startDim2, 0.1f, 30.0f, 5.0f);
    sliderWithReset("End Dim1", s.endDim1, 0.1f, 30.0f, 5.0f);
    sliderWithReset("End Dim2", s.endDim2, 0.1f, 30.0f, 5.0f);
    ImGui::SeparatorText("Easing Splines Editors");
    bezierEditor("Speed", s.easingSpeed);
    ImGui::SameLine();
    bezierEditor("Angle", s.easingAngle);
    ImGui::SameLine();
    bezierEditor("Dim1", s.easingDim1);
    ImGui::SameLine();
    bezierEditor("Dim2", s.easingDim2);

    // Timing and attack creation
    static auto bodyEntity = Game::registry.create();

    static float lastTime = GetTime() - duration;
    if ((lastTime + duration + interval) < GetTime()) {
        lastTime = GetTime();

        // Create new body entity
        bodyEntity = Game::registry.create();
        auto body = createBody(bodyEntity);
        auto box = b2MakeBox(s.startDim1, s.startDim2);
        createShape(body, box);

        // Link to player and setup attack components
        Game::registry.emplace<Attacks::Attack>(bodyEntity, Attacks::Attack{100.0f});
        Game::registry.emplace<Attacks::BodyCouple>(bodyEntity, Attacks::BodyCouple{playerBody, body});
        Game::registry.emplace<PassiveTimer>(bodyEntity, duration);
    }
    if (Game::registry.valid(bodyEntity))
        Game::registry.emplace_or_replace<Attacks::Transform>(bodyEntity, s);


    // Timing sliders
    ImGui::SliderFloat("Duration", &duration, 0.1f, 10.0f);
    ImGui::SliderFloat("Interval", &interval, 0.0f, 10.0f);
    ImGui::End();
}

void imguiEnemyTypesEditor() {
    ImGui::Begin("Enemy Types Editor");
    Vector2 playerPosition = {0, 0};
    if (!Game::registry.view<Player>().empty()) {
        const auto player = Game::registry.view<Player>().front();
        playerPosition = Game::registry.get<Position>(player);
    }


    static auto enemyList = Assets::GetEnemiesData().enemyStats;
    if (ImGui::Button("Save")) {
        EnemyDataFile::saveCSV(enemyList);
        Assets::InstantiateEnemiesFile();
    }

    if (ImGui::BeginTable("EnemyTable", EnemyDataFile::headers.size() + 1,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
        // Render table headers
        ImGui::TableSetupColumn("Spawn");
        for (const char *header: EnemyDataFile::headers) {
            ImGui::TableSetupColumn(header);
        }
        ImGui::TableHeadersRow();

        // Render rows for each enemy in the list
        for (size_t i = 0; i < enemyList.size(); ++i) {
            EnemyType &enemy = enemyList[i];
            ImGui::TableNextRow();


            ImGui::TableNextColumn();
            if (Config::GetBool("in_editor_level")) ImGui::BeginDisabled();
            if (ImGui::Button(("Spawn##" + std::to_string(i)).c_str())) {
                spawnEnemyByName(playerPosition + Vector2{0.0f, 40.0f}, enemy.name);
            }
            if (Config::GetBool("in_editor_level")) ImGui::EndDisabled();
            char buffer[80];
            std::strncpy(buffer, enemy.name.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';
            ImGui::TableNextColumn();
            ImGui::InputText(("##Name" + std::to_string(i)).c_str(), buffer, 80);
            enemy.name = std::string(buffer);
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
                100.0f, 10, "", ""
        });
    }
    ImGui::End();
}

void imguiLevel() {
    ImGui::Begin("Level");
    static int level{Level::First};
    if (ImGui::InputInt("Select Level", &level)) {
        Game::ChangeLevel(static_cast<Level::LevelName>(level));
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

void imguiLocalSpace(const Camera2D &camera) {
    auto player = Game::registry.view<Player>().front();
    auto body = Game::registry.get<b2BodyId>(player);
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

void imguiEnemyAttr() {
    ImGui::Begin("Enemy attributes");
    ImGui::Checkbox("show_astar_path", Config::GetBoolPtr("show_astar_path"));
    ImGui::Checkbox("show_enemy_fov", Config::GetBoolPtr("show_enemy_fov"));
    ImGui::Checkbox("show_enemy_texture", Config::GetBoolPtr("show_enemy_texture"));
    ImGui::SeparatorText("Selected Enemy attributes");

    auto view = Game::registry.view<Enemy, Selected, Path, ID, ColorBB, Spread, Speed, Health, Radius, AttackTimer,
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
            ImGui::TextColored(color, "%s", name);
            ImGui::PopID();
            n++;
        }
    }
    ImGui::End();
}

void imguiPlayerAttr() {
    ImGui::Begin("Player values");

    auto view = Game::registry.view<Player, ColorBB, Spread, Speed, Health, Radius, AttackTimer, AttackSpeed, Damage,
            AttackRange, Pushback, Position>();
    for (auto [entity, colorbb, spread, speed, health, radius, timelastattack,
                attackspeed, damage, attackrange, pushback, position]: view.each()) {
        ImGui::SliderFloat("health", &health.value, 0, 200, "%.3f", 0);
        ImGui::SliderFloat("radius", &radius.value, 0, 50, "%.3f", 0);
        ImGui::SliderFloat("position x", &position.x, 0, Const::mapWidth, "%.3f", 0);
        ImGui::SliderFloat("position y ", &position.y, 0, Const::mapHeight, "%.3f", 0);
    }
    ImGui::End();
}

void imguiAttributesMultipliers() {
    ImGui::Begin("Attribute multipliers");
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); }
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::MultiplierOriginal(subattr) ==
                        Params::Multiplier(subattr);
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::Multiplier(subattr) = Params::MultiplierOriginal(subattr);
        }
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

void imguiAttributes() {
    ImGui::Begin("Attributes");

    int n = {0};
    int m = {0};
    bool disableUp{false};
    bool disableDown{false};
    auto view = Game::registry.view<Player, Experience, Attributes>();
    for (auto [entity, experience, attributes]: view.each()) {
        ImGui::Text("Level %d", attributes.getLevel());
        ImGui::SameLine();
        ImGui::ProgressBar(static_cast<float>(experience - attributes.expForCurrentLevel()) /
                           static_cast<float>(attributes.expToNextLevel() - attributes.expForCurrentLevel()));
        ImGui::Text("Exp %d", experience.value);

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

void imguiInventory() {
    ImGui::Begin("Inventory");

    auto view = Game::registry.view<OnPlayer, Item, Name, AttributeConstants::Modifier>();
    for (auto [entity, name, modifier]: view.each()) {
        ImGui::Text("%s", name.value.c_str());
        ImGui::SameLine();
        if (ImGui::Button("Drop")) {
            for (auto [player, position]: Game::registry.view<Player, Position>().each()) {
                Game::registry.remove<OnPlayer>(entity);
                Game::registry.emplace<Position>(entity, Vector2Add(position, {10, 10}));
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
    if (ImGui::Button("Save")) { Config::SaveAttributeParameters(); }
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
    if (ImGui::Button("Save")) { Params::SaveAttributeParameters(); }
    int n = {0};
    for (auto subattr: AttributeConstants::subAttributeVec) {
        ImGui::PushID(n);
        bool disableR = Params::StartValue(subattr) ==
                        Params::StartValueOriginal(subattr);
        if (disableR) ImGui::BeginDisabled(true);
        if (ImGui::Button("R")) {
            Params::StartValue(subattr) = Params::StartValueOriginal(subattr);
        }
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

template<typename Func, typename... Args>
void showChecked(const char *label, Func &&func, Args &&... args) {
    static std::unordered_map<std::string, bool> checkboxStates;
    bool &show_window = checkboxStates[label];
    ImGui::Checkbox(label, &show_window);
    if (show_window) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }
}

template<typename Func, typename... Args>
void showCheckedGlobal(const char *label, Func &&func, bool *show_window, Args &&... args) {
    ImGui::Checkbox(label, show_window);
    if (*show_window) {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }
}

void imguiWindowMain(const Camera2D &camera) {
    static auto inEditor = Config::GetBoolPtr("inEditor");

    ImGui::Begin("Main");

    ImGui::Checkbox("Spline Window", Config::GetBoolPtr("show_spline_ui"));
    if (Config::GetBool("show_spline_ui")) {
        *inEditor = true;
        imguiSplineEditor(camera);
    } else
        *inEditor = false;

    showCheckedGlobal("Level Window", imguiLevel, Config::GetBoolPtr("show_menu"));

    showChecked("Assets Window", imguiShowAssets);
    showChecked("Player Window", imguiPlayerAttr);
    showChecked("Cursor Window", imguiCursor, camera);
    showChecked("Local Space Window", imguiLocalSpace, camera);
    showChecked("Enemy Window", imguiEnemyAttr);
    showChecked("Enemy Types Window", imguiEnemyTypesEditor);
    showChecked("Config Window", imguiConfig);
    showChecked("Attributes Window", imguiAttributes);
    showChecked("Inventory Window", imguiInventory);
    showChecked("Instructions", imguiInstructions);
    showChecked("Multipliers Window", imguiAttributesMultipliers);
    showChecked("Start Values Window", imguiSubAttributesStartValues);


    static bool droppedFile{false};
    static FilePathList files{};
    if (IsFileDropped()) droppedFile = true;
    if (droppedFile) {
        files = LoadDroppedFiles();
        SPDLOG_INFO("Dropped a file: ");
        SPDLOG_INFO(files.paths[files.count - 1]);
        auto filename = files.paths[files.count - 1];
        auto filepath = std::filesystem::path(filename);
        if (filepath.extension() == ".mp3")
            modalDroppedMp3(filepath, &droppedFile);
        else droppedFile = false;
    }
    if (!droppedFile && files.count > 0) {
        UnloadDroppedFiles(files);
        files.count = 0;
    }


    ImGui::End();
}


void imguiWindowMainAI(const Camera2D &camera) {
    ImGui::Begin("Main");

    showCheckedGlobal("Level Window", imguiLevel, Config::GetBoolPtr("show_menu"));

    showChecked("Assets Window", imguiShowAssets);
    showChecked("Cursor Window", imguiCursor, camera);
    showChecked("Local Space Window", imguiLocalSpace, camera);
    showChecked("Enemy Types Window", imguiEnemyTypesEditor);
    showChecked("Config Window", imguiConfig);
    showChecked("Instructions", imguiInstructions);

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

void Gui::Update(const Camera2D &camera, const std::function<void(const Camera2D &)> &mainFunc = &imguiWindowMain) {
    ImGui_ImplRaylib_ProcessEvents();
    ImGui_ImplRaylib_NewFrame();
    ImGui::NewFrame();

    mainFunc(camera);
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

void Gui::Clean() {
    ImGui_ImplRaylib_Shutdown();
    ImGui::DestroyContext();
}