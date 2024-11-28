//
// Created by ginko on 11/26/24.
//

#include "player_ui.h"
#include <constants.h>
#include "managers/game.h"

namespace PlayerUI {
    void Draw(const float health) {
        DrawFPS(10, 10);
        DrawText(std::format("Health: {}", health).c_str(), 10, Const::screenHeight - 40, 30, WHITE);
        // TODO make a PlayerUI namespace and just PlayerUI::Draw() everything
        if (Game::IsPaused()) {
            // TODO this feels like it should be Game::IsPaused(), put it in a namespace for now
            DrawText("PAUSE", Const::screenHeight / 2 - 50, Const::screenWidth / 2 - MeasureText("PAUSE", 50), 50, WHITE);
        }
    }
}
