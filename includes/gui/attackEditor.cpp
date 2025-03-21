//
// Created by ginko on 12/10/24.
//

#include "attackEditor.h"
#include "systems/physics.h"
#include "bezierEditor.h"
#include "math/spline.h"
#include "managers/game.h"
#include "systems/attacks.h"
#include "timer.h"
#include "components.h"

//
//auto createBody = [](const entt::entity entity) {
//    b2BodyDef bodyDef = b2DefaultBodyDef();
//    bodyDef.type = b2_kinematicBody;
//    const b2BodyId body = b2CreateBody(Physics::GetWorldID(), &bodyDef);
//    Physics::ConnectBodyToEntity(body, entity);
//    return body;
//};

//auto createShape = [](b2BodyId bodyId, b2Polygon box) {
//    b2ShapeDef shapeDef = b2DefaultShapeDef();
//    shapeDef.isSensor = true;
//    shapeDef.filter.maskBits = Physics::Enemy;
//    return b2CreatePolygonShape(bodyId, &shapeDef, &box);
//};

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
    ImGui::PushID(entt::hashed_string(label).value());
    if (ImGui::Button("R")) value = resetValue;
    ImGui::SameLine();
    ImGui::SliderFloat(label, &value, min, max);
    ImGui::PopID();
};


void imguiSplineEditor(const Camera2D &camera) {
    // Static variables for spline editor state
    static std::array<b2Vec2, 4> points{};
    static std::array<bool, 4> pointsCreated = {false};
    static std::array<bool, 4> pointsMoving = {false};



    // Get mouse position in world coordinates
    const Math::Vec2 mouse = GetScreenToWorld2D(GetMousePosition(), camera);

    // Get player body and its transform
    const auto player = Game::registry.view<Player>().front();
    const auto playerBody = Game::registry.get<b2BodyId>(player);
    const auto playerPosition = b2Body_GetPosition(playerBody);
    const auto fixedPlayerTransform = b2Transform(playerPosition, b2Rot(cos(-1.5708), sin(-1.5708)));



    // Create and update the local spline
    std::array<Math::Vec2, 4> trajPoints{};
    for (size_t i{0}; i < 4; i++)
        trajPoints[i] = points[i];
    auto trajectory = Spline{trajPoints};

    // Handle mouse input for creating the first two points
    if (!pointsCreated[0] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[0] = pointsCreated[1] = true;
        points[0] = b2InvTransformPoint(fixedPlayerTransform, mouse);
        points[1] = b2Add(points[0], {20, 20});
    }
        // Handle mouse input for creating the last two points
    else if (!pointsCreated[3] && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        pointsCreated[2] = pointsCreated[3] = true;
        points[3] = b2InvTransformPoint(fixedPlayerTransform, mouse);
        points[2] = b2Add(points[3], {20, 20});
    }


    // Check for point movement
    for (int i = 0; i < 4; i++) {
        if (CheckCollisionPointCircle(
                Math::Vec2(b2TransformPoint(fixedPlayerTransform, Math::Vec2(trajectory.bezier[i]))), mouse, 5.0f) &&
            IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            pointsMoving[i] = true;
            break;
        }
    }
    for (int i = 0; i < 4; i++) {
        if (pointsMoving[i]) {
            points[i] = b2InvTransformPoint(fixedPlayerTransform, mouse);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                pointsMoving[i] = false;
            }
        }
    }

    // Start ImGui spline editor window
    ImGui::Begin("Spline editor");


    // Spline parameters
    static float duration = 2.0f;
    static float interval = 0.0f;

    // Editor for Transform parameters
    static auto transform = Attacks::Transform{trajectory};
    transform.trajectory = trajectory;

    ImGui::SeparatorText("Parameters Editor");
    sliderWithReset("Start Radians", transform.startRadians, -3.14f, 3.14f, 0.0f);
    sliderWithReset("End Radians", transform.endRadians, -3.14f, 3.14f, 0.0f);
    sliderWithReset("Start Dim1", transform.startDim1, 0.1f, 30.0f, 5.0f);
    sliderWithReset("Start Dim2", transform.startDim2, 0.1f, 30.0f, 5.0f);
    sliderWithReset("End Dim1", transform.endDim1, 0.1f, 30.0f, 5.0f);
    sliderWithReset("End Dim2", transform.endDim2, 0.1f, 30.0f, 5.0f);
    ImGui::SeparatorText("Easing Splines Editors");
    bezierEditor("Speed", transform.easingSpeed);
    ImGui::SameLine();
    bezierEditor("Angle", transform.easingAngle);
    ImGui::SameLine();
    bezierEditor("Dim1", transform.easingDim1);
    ImGui::SameLine();
    bezierEditor("Dim2", transform.easingDim2);

    // Timing and attack creation
    static auto bodyEntity = Game::registry.create();

    static float lastTime = GetTime() - duration;
    if ((lastTime + duration + interval) < GetTime()) {
        lastTime = GetTime();

        // Create new body entity
        bodyEntity = Game::registry.create();
        auto body = Attacks::createBody(bodyEntity);
        auto box = b2MakeBox(transform.startDim1, transform.startDim2);
        Attacks::createShape(body, box);

        // Link to player and setup attack components
        Game::registry.emplace<Attacks::Attack>(bodyEntity, Attacks::Attack{100.0f});
        Game::registry.emplace<Attacks::BodyCouple>(bodyEntity, Attacks::BodyCouple{playerBody, body});
        Game::registry.emplace<PassiveTimer>(bodyEntity, duration);
    }
    if (Game::registry.valid(bodyEntity))
        Game::registry.emplace_or_replace<Attacks::Transform>(bodyEntity, transform);

    if (ImGui::BeginPopupContextItem("my popup"))
    {
        constexpr const char * nullName = "none";
        const char * name = nullName;
        for (const auto& file: std::filesystem::directory_iterator(std::filesystem::path(ROOT_PATH) / std::filesystem::path(ATTACK_PATH))){
            auto filename = file.path().filename().replace_extension("").c_str();
            if (ImGui::Selectable(filename)) {
                for (size_t i{0}; i < 4; i++) {
                    transform.loadFromTOML(filename);
                    points[i] = transform.trajectory.get()[i];
                    pointsCreated[i] = true;
                }
            }
        }
        ImGui::EndPopup();
    }

    // Timing sliders
    ImGui::SliderFloat("Duration", &duration, 0.1f, 10.0f);
    ImGui::SliderFloat("Interval", &interval, 0.0f, 10.0f);

    char buffer[80];
    std::strncpy(buffer, transform.name.c_str(), sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    if (ImGui::InputText("AttackName", buffer, 80))
        transform.name = buffer;
    if (ImGui::Button("Save attack")) transform.saveToTOML();
    if (ImGui::Button("Load attack")) {
        ImGui::OpenPopup("my popup");
    }

    ImGui::End();
}
