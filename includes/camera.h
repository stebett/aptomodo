//
// Created by ginko on 11/28/24.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <factories.h>


class GameCamera {
private:
    Camera2D playerCamera;
    Camera2D freeCamera;
    Camera2D *activeCamera;

public:
    operator Camera2D() const noexcept;

    GameCamera();

    void FollowPlayer(const Vector2 &playerPosition, float delta);

    const Camera2D &GetPlayerCamera() const;

    void Update(const Vector2 &playerPosition, float delta);

    void Zoom() const;

    void MoveFreeCamera(const Vector2 &movement);
};


#endif //CAMERA_H
