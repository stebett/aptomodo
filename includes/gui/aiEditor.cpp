//
// Created by ginko on 12/7/24.
//

#include "aiEditor.h"

Default View::defaultState;
MovingView View::movingView;

class Grid {
    static constexpr int gridEdge = 4;
    static constexpr int gridSize = 500;
    RenderTexture2D grid{LoadRenderTexture(gridSize * gridEdge, gridSize * gridEdge)};

public:
    Grid() {
        BeginTextureMode(grid);
        for (int i{-gridSize}; i <= gridSize; i++) {
            int start = -gridSize * gridEdge;
            int j = i * gridEdge;
            DrawLineEx(Vector2(start, j), Vector2(-start, j), 1, ColorAlpha(WHITE, 0.3));
            DrawLineEx(Vector2(j, start), Vector2(j, -start), 1, ColorAlpha(WHITE, 0.3));
        }
        EndTextureMode();
    };

    void Draw() {
        DrawTexture(grid.texture, -round(gridSize * gridEdge / 2), -round(gridSize * gridEdge / 2), RAYWHITE);
    }
};

//enum State {
//    None,
//    MovingView,
//    CreatingState,
//    HoveringState,
//    CreatingTransition,
//    CreatingBehavior
//};

Camera2D camera{};


void Default::handleInput(View &view) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        view.state = &View::movingView;
}

void Default::onEnter(View &view) {
    view.state = &View::defaultState;
}

void MovingView::handleInput(View &view) {
    camera.target += start - GetScreenToWorld2D(GetMousePosition(), camera);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) view.state = &View::defaultState;
}

void MovingView::onEnter(View &view) {
    view.state = &View::movingView;
    start = GetScreenToWorld2D(GetMousePosition(), camera);
}

void View::handleInput() {
    state->handleInput(*this);
}


LevelOutcome PlayAIEditor() {
    SPDLOG_INFO("Entering Editor Level");
    Game::EnterLevel();
    auto grid{Grid()};
    View view;

    camera.target = {0, 0};
    camera.zoom = 3.4;
    Game::SetOutcome(LevelOutcome::QUIT);

    SetTargetFPS(60);
    while (!Game::IsLevelFinished()) {
        camera.zoom += GetMouseWheelMove() / 10;
        view.handleInput();
        if (IsKeyPressed(KEY_Q)) Game::ExitLevel();

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(DARKGRAY);
        grid.Draw();

        EndMode2D();

        EndDrawing();

    }
    return Game::GetOutcome();
}
