#include "includes/constants.h"
#include "includes/managers/configManager.h"
#include "includes/managers/animationManager.h"
#include "includes/managers/assets.h"
#include "includes/managers/game.h"
#include "includes/managers/logging.h"
#include "includes/managers/parameters.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main() {
    Log::Instantiate();
    InitWindow(Const::screenWidth, Const::screenHeight, "Apto Modo");
    ToggleFullscreen();
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    Params::Instantiate();
    SPDLOG_INFO("Finished instantiation");

    Game::ChangeLevel(static_cast<Level::LevelName>(Config::GetInt("starting_level")));
    Game::Loop();
    CloseWindow();
    Assets::Clean();
    return 0;
}
