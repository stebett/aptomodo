//
// Created by ginko on 11/28/24.
//

#include "camera.h"

#include "managers/configManager.h"
#include <managers/gui.h>
#include <math/decay.h>

GameCamera::operator Camera2D() const noexcept {
    return *activeCamera;
}

GameCamera::GameCamera() {
    playerCamera = spawnCamera();
    freeCamera = spawnCamera();
    activeCamera = &playerCamera;
}

void GameCamera::FollowPlayer(const Vector2 &playerPosition, const float delta) {
    float target_x = playerPosition.x;
    float target_y = playerPosition.y;

    // Don't show map end
    if (playerPosition.x < Const::screenWidth / (2 * activeCamera->zoom)) {
        target_x = Const::screenWidth / (2 * activeCamera->zoom);
    }
    if (playerPosition.x > Const::mapWidth - Const::screenWidth / (2 * activeCamera->zoom)) {
        target_x = Const::mapWidth - Const::screenWidth / (2 * activeCamera->zoom);
    }
    if (playerPosition.y < Const::screenHeight / (2 * activeCamera->zoom)) {
        target_y = Const::screenHeight / (2 * activeCamera->zoom);
    }
    if (playerPosition.y > Const::mapHeight - Const::screenHeight / (2 * activeCamera->zoom)) {
        target_y = Const::mapHeight - Const::screenHeight / (2 * activeCamera->zoom);
    }

    target_x = expDecay(activeCamera->target.x, target_x, Config::GetFloat("cameraDecay"), delta);
    target_y = expDecay(activeCamera->target.y, target_y, Config::GetFloat("cameraDecay"), delta);
    activeCamera->target = {target_x, target_y};
}

const Camera2D &GameCamera::GetPlayerCamera() const {
    return playerCamera;
}

Camera2D &GameCamera::GetPlayerCamera() {
    return playerCamera;
}


void GameCamera::Update(const Vector2 &playerPosition, const float delta) {
    if (Config::GetBool("free_camera")) {
        activeCamera = &freeCamera;
    } else {
        activeCamera = &playerCamera;
        freeCamera = playerCamera;
        FollowPlayer(playerPosition, delta);
    }
    if (!Gui::WantMouse())
        Zoom();
}

void GameCamera::Zoom() const {
    activeCamera->zoom += GetMouseWheelMove() / 10;
}

void GameCamera::MoveFreeCamera(const Vector2 &movement) {
    freeCamera.target = Vector2Add(freeCamera.target, movement);
}

