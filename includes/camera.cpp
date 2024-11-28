//
// Created by ginko on 11/28/24.
//

#include "camera.h"

#include <config.h>
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
    if (playerPosition.x < screenWidth / (2 * activeCamera->zoom)) {
        target_x = screenWidth / (2 * activeCamera->zoom);
    }
    if (playerPosition.x > mapWidth - screenWidth / (2 * activeCamera->zoom)) {
        target_x = mapWidth - screenWidth / (2 * activeCamera->zoom);
    }
    if (playerPosition.y < screenHeight / (2 * activeCamera->zoom)) {
        target_y = screenHeight / (2 * activeCamera->zoom);
    }
    if (playerPosition.y > mapHeight - screenHeight / (2 * activeCamera->zoom)) {
        target_y = mapHeight - screenHeight / (2 * activeCamera->zoom);
    }

    target_x = expDecay(activeCamera->target.x, target_x, Config::GetFloat("CameraDecay"), delta);
    target_y = expDecay(activeCamera->target.y, target_y, Config::GetFloat("CameraDecay"), delta);
    activeCamera->target = {target_x, target_y};
}

const Camera2D &GameCamera::GetPlayerCamera() const {
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
    Zoom();
}

void GameCamera::Zoom() const {
    activeCamera->zoom += GetMouseWheelMove() / 10;
}

void GameCamera::MoveFreeCamera(const Vector2 &movement) {
    freeCamera.target = Vector2Add(freeCamera.target, movement);
}