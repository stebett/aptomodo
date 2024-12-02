//
// Created by ginko on 12/2/24.
//

#include "animationEditor.h"
#include "rlImGui.h"
#include "managers/game.h"

constexpr int gridEdge = 4;
constexpr int gridSize = 500;

void ShowAnimationEditor() {
    ImGui::Begin("Animation Editor");
    if (IsFileDropped()) {
        auto file{LoadDroppedFiles()};
        auto path = std::filesystem::path(file.paths[0]);
        SPDLOG_INFO("Loading file {}", path.string());
        static auto texture = LoadTexture(path.c_str());
        rlImGuiImage(&texture);
    }
    ImGui::End();
}

int roundUpToNextMultiple(float value, int n) {
    if (n == 0) return value; // Avoid division by zero
    return round(value / n) * n;
}

Rectangle RectangleFromTwoPoints(std::array<Vector2, 2> points) {
    const float xMin = std::min(points[0].x, points[1].x);
    const float yMin = std::min(points[0].y, points[1].y);
    const float width = std::abs(points[0].x - points[1].x);
    const float height = std::abs(points[0].y - points[1].y);

    return Rectangle(xMin,
                     yMin,
                     width,
                     height);
}

enum class Direction {
    Horizontal,
    Vertical,
    None,
};


void SelectRectangles(const Camera2D &camera, std::vector<Rectangle> &recs) {
    static const Color baseColor = WHITE;
    static const Color selectedColor = PURPLE;
    static std::array<Vector2, 2> points{};
    static Vector2 *pointMoving = {};
    static Rectangle *recMoving = nullptr;

    for (size_t i{0}; i < recs.size(); i++) {
        Color col = i == recs.size() - 1 ? selectedColor : baseColor;
        DrawRectangleLinesEx(recs[i], 1, col);
    }
    if (pointMoving) {
        DrawRectangleLinesEx(RectangleFromTwoPoints(points), 1, baseColor);
    }
    if (points[0] != Vector2Zeros || points[1] != Vector2Zeros) {
        DrawCircleV(points[0], 1, BLUE);
        DrawCircleV(points[1], 1, RED);
    }

    static Vector2 lastMouse{};
    Vector2 newMouse{GetScreenToWorld2D(GetMousePosition(), camera)};

    static Vector2 mouse = newMouse;
    static Direction direction = Direction::None;
    if (direction == Direction::None && IsKeyDown(KEY_LEFT_CONTROL) && (pointMoving || recMoving)) {
        auto [x, y] = newMouse - mouse;
        if (abs(x) < abs(y)) direction = Direction::Vertical;
        if (abs(x) > abs(y)) direction = Direction::Horizontal;
    }
    if (direction == Direction::Vertical) newMouse.x = mouse.x;
    if (direction == Direction::Horizontal) newMouse.y = mouse.y;
    if (IsKeyReleased(KEY_LEFT_CONTROL)) direction = Direction::None;
    mouse = Vector2(roundUpToNextMultiple(newMouse.x, gridEdge), roundUpToNextMultiple(newMouse.y, gridEdge));


    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
        DrawRectangleLinesEx(recs.back(), 1, RED);
        recs.pop_back();
        points[0] = Vector2Zeros;
        points[1] = Vector2Zeros;
    }
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V) && !recs.empty()) {
        auto newRec = recs.back();
        newRec.x = mouse.x;
        newRec.y = mouse.y;
        recs.push_back(newRec);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (!pointMoving && !recMoving) {
            for (auto &point: points) {
                if (CheckCollisionPointCircle(point, mouse, 3)) {
                    recs.pop_back();
                    pointMoving = &point;
                    break;
                }
            }
        }

        if (!pointMoving && !recMoving) {
            bool success = false;
            size_t index{};
            for (size_t i{0}; i < recs.size(); i++) {
                if (CheckCollisionPointRec(mouse, recs[i])) {
                    success = true;
                    lastMouse = mouse;
                    index = i;
                    break;
                }
            }
            if (success) {
                std::rotate(recs.begin(), recs.begin() + index + 1, recs.end());
                recMoving = &recs.back();
            }

        }

        if (!pointMoving && !recMoving) {
            points[0] = mouse;
            points[1] = mouse;
            pointMoving = &points[1];
        }
    }

    if (pointMoving) {
        *pointMoving = mouse;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (Vector2Distance(points[0], points[1]) > 5) {
                recs.push_back(RectangleFromTwoPoints(points));
            } else {
                points[0] = Vector2Zeros;
                points[1] = Vector2Zeros;
            }
            pointMoving = nullptr;

        }
    }

    if (recMoving) {
        recMoving->x += mouse.x - lastMouse.x;
        recMoving->y += mouse.y - lastMouse.y;

        // Define vertices of the moving rectangle
        auto movingTopLeft = Vector2{recMoving->x, recMoving->y};
        auto movingTopRight = Vector2{recMoving->x + recMoving->width, recMoving->y};
        auto movingBottomLeft = Vector2{recMoving->x, recMoving->y + recMoving->height};

        points[0] = movingBottomLeft;
        points[1] = movingTopRight;
        lastMouse = mouse;

        float threshold = 3.0f;
        for (auto rec: recs) {
            if (rec.x == recMoving->x && rec.y == recMoving->y) continue;

            // Define vertices of the current rectangle
            auto topLeft = Vector2{rec.x, rec.y};
            auto topRight = Vector2{rec.x + rec.width, rec.y};
            auto bottomLeft = Vector2{rec.x, rec.y + rec.height};



            // Case 1: top-right of moving to top-left of another
            if (abs(Vector2Distance(movingTopRight, topLeft)) < threshold) {
                recMoving->x = topLeft.x - recMoving->width;
                recMoving->y = topLeft.y;
            }
                // Case 2: top-left of moving to top-right of another
            else if (abs(Vector2Distance(movingTopLeft, topRight)) < threshold) {
                recMoving->x = topRight.x;
                recMoving->y = topRight.y;
            }
                // Case 3: bottom of moving to top of another
            else if (abs(Vector2Distance(movingBottomLeft, topLeft)) < threshold) {
                recMoving->x = topLeft.x;
                recMoving->y = topLeft.y - recMoving->height;
            }
                // Case 4: top of moving to bottom of another
            else if (abs(Vector2Distance(movingTopLeft, bottomLeft)) < threshold) {
                recMoving->x = bottomLeft.x;
                recMoving->y = bottomLeft.y;
            }

        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            recMoving = nullptr;
        }
    }
}

void PlayAnimation(Animation anim, Vector2 pos, double t) {
    if (anim.frameCount < 1) return;
    static int i = 0;
    static double start = t;
    if (t - start > 0.2) {
        start = t;
        i = (i + 1) % anim.frameCount;
    }
    DrawTextureRec(anim.texture, anim.sources[i], pos, WHITE);
}

LevelOutcome AnimationEditorLevel(Camera2D &camera) {
    static Texture2D texture = {};
    static bool draw{false};
    if (!draw && IsFileDropped()) {
        auto file{LoadDroppedFiles()};
        auto path = std::filesystem::path(file.paths[0]);
        SPDLOG_INFO("Loading file {}", path.string());
        texture = LoadTexture(path.c_str());
        draw = true;
        float textureLargerDimension = std::max(texture.width, texture.height);
        std::array<int, 2> screenDimensions = {GetScreenWidth(), GetScreenHeight()};
        int dim = texture.width > texture.height ? 0 : 1;

        float zoomFactor = screenDimensions[dim] / textureLargerDimension * 0.75f;


        camera.zoom = zoomFactor;
        Vector2 screenDimensionsCorrected = GetScreenToWorld2D(Vector2(GetScreenWidth(), GetScreenHeight()), camera);

        camera.target = Vector2(0, 0);
        camera.offset = screenDimensionsCorrected / 2;


    }
    if (draw) DrawTexture(texture, 0, 0, RAYWHITE);

    static bool done{false};
    static std::vector<Rectangle> recs;
    if (!done) SelectRectangles(camera, recs);
    if (IsKeyPressed(KEY_ENTER)) done = true;
    if (IsKeyPressed(KEY_BACKSPACE)) done = false;

    auto anim = Animation(texture, recs.size(), recs);
    if (!recs.empty())
        PlayAnimation(anim, Vector2(texture.width / 2, texture.height + 10), GetTime());
    return LevelOutcome::WIN;
}


/*
 * Different levels should
 *  - load different ldtk::levels
 *  - have different gui windows preopened
 */
LevelOutcome PlayAnimationEditorLevel() {
    Game::EnterLevel();
    SPDLOG_INFO("Entering level");
    Camera2D camera{};
    camera.target = {0, 0};
    camera.zoom = 3.4;
    Game::SetOutcome(LevelOutcome::QUIT);
    RenderTexture2D grid{LoadRenderTexture(gridSize * gridEdge, gridSize * gridEdge)};
    BeginTextureMode(grid);
    for (int i{-gridSize}; i <= gridSize; i++) {
        int start = -gridSize * gridEdge;
        int j = i * gridEdge;
        DrawLineEx(Vector2(start, j), Vector2(-start, j), 1, ColorAlpha(WHITE, 0.3));
        DrawLineEx(Vector2(j, start), Vector2(j, -start), 1, ColorAlpha(WHITE, 0.3));
    }
    EndTextureMode();
    SetTargetFPS(60);
    while (!Game::IsLevelFinished()) {
        camera.zoom += GetMouseWheelMove() / 10;
        std::bitset<4> bitfield;
        if (!Game::IsPaused()) {
            if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) bitfield.set(0);
            if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) bitfield.set(1);
            if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) bitfield.set(2);
            if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) bitfield.set(3);
        }
        if (IsKeyPressed(KEY_Q)) Game::ExitLevel();
        camera.target.y -= static_cast<float>(bitfield[0]);
        camera.target.y += static_cast<float>(bitfield[1]);
        camera.target.x -= static_cast<float>(bitfield[2]);
        camera.target.x += static_cast<float>(bitfield[3]);

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(DARKGRAY);
        DrawTexture(grid.texture, -round(gridSize * gridEdge / 2), -round(gridSize * gridEdge / 2), RAYWHITE);
        AnimationEditorLevel(camera);
        EndMode2D();
        EndDrawing();
    }

    return Game::GetOutcome();
}

