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
    try {
        for (const auto &entry: std::filesystem::recursive_directory_iterator(ROOT_PATH)) {
            std::cout << entry.path() << std::endl;
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
//    ToggleFullscreen();
    Config::Instantiate();
    Assets::Instantiate();
    AnimationManager::Instantiate();
    Params::Instantiate();
    SPDLOG_INFO("Finished instantiation");
    Game::Level = 2;
    Game::PrepareLevel(2);
    emscripten_set_main_loop(Game::LoopOnce, 0, 1);
    Game::CleanLevel();
//    Game::Loop();
    CloseWindow();
    Assets::Clean();
    return 0;
}
