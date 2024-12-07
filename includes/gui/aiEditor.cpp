//
// Created by ginko on 12/7/24.
//

#include "aiEditor.h"
#include "math/helpers.h"

Default View::defaultState;
MovingView View::movingView;
HoveringState View::hoveringState;
static constexpr int gridEdge = 4;
static constexpr int gridSize = 500;


class Grid {
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
using AnyWidget = std::variant<Widgets::State>;

Camera2D camera{};
Vector2 mouse{};
Vector2 mouseGrid{};

std::vector<Widgets::State> stateWidgets{};


void View::handleInput() {
    state->handleInput(*this);
}

void DrawWidgets() {
    for (size_t i{0}; i < stateWidgets.size(); i++) {
        if (i == stateWidgets.size()-1) stateWidgets[i].drawSelected();
        else stateWidgets[i].draw();
    }
}

LevelOutcome PlayAIEditor() {
    SPDLOG_INFO("Entering Editor Level");
    Game::EnterLevel();
    auto grid{Grid()};
    View view;

    camera = {};
    stateWidgets.clear();


    camera.target = {0, 0};
    camera.zoom = 3.4;
    Game::SetOutcome(LevelOutcome::QUIT);

    SetTargetFPS(60);
    while (!Game::IsLevelFinished()) {
        mouse = GetScreenToWorld2D(GetMousePosition(), camera);
        mouseGrid = Vector2(roundUpToNextMultiple(mouse.x, gridEdge), roundUpToNextMultiple(mouse.y, gridEdge));

        camera.zoom += GetMouseWheelMove() / 10;
        view.handleInput();
        if (IsKeyPressed(KEY_Q)) Game::ExitLevel();
        if (IsKeyPressed(KEY_R)) {
            Game::SetOutcome(LevelOutcome::RESTART);
            Game::ExitLevel();
        }

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(DARKGRAY);
        grid.Draw();
        DrawWidgets();

        EndMode2D();

        EndDrawing();

    }
    return Game::GetOutcome();
}

void swap(Widgets::State &lhs, Widgets::State &rhs) noexcept {
    lhs.swap(rhs);
}

void Default::handleInput(View &view) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        View::movingView.enter(view);
    for (size_t i{0}; i < stateWidgets.size(); i++)
        if (CheckCollisionPointCircle(mouse, stateWidgets[i].position, stateWidgets[i].radius)) {
            std::rotate(stateWidgets.begin(), stateWidgets.begin() + i + 1, stateWidgets.end());
            View::hoveringState.enter(view);
            return;
        }
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        stateWidgets.emplace_back(mouseGrid);
        View::hoveringState.enter(view);
    }
}

void Default::enter(View &view) {
    view.state = &View::defaultState;
}

void MovingView::handleInput(View &view) {
    camera.target += start - mouse;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) View::defaultState.enter(view);;
}

void MovingView::enter(View &view) {
    view.state = &View::movingView;
    start = mouse;
}

void HoveringState::handleInput(View &view) {
    if (!CheckCollisionPointCircle(mouse, stateWidgets.back().position, stateWidgets.back().radius)) {
        View::defaultState.enter(view);
    }
}

void HoveringState::enter(View &view) {
    view.state = &View::hoveringState;
}
