//
// Created by ginko on 20/05/24.
//

#ifndef APTOMODO_FRAMERATEMANAGER_H
#define APTOMODO_FRAMERATEMANAGER_H

#include <raylib.h>
#include "../config.h"

struct FramerateManager {

    unsigned int framesCounter = 0u;
    // Custom timing variables
    double previousTime = GetTime();    // Previous time measure
    double currentTime = 0.0;           // Current time measure
    double updateDrawTime = 0.0;        // Update + Draw time
    double waitTime = 0.0;              // Wait time (if target fps required)
    float deltaTime = 0.0f;             // Frame time (Update + Draw + Wait time)

    float timeCounter = 0.0f;           // Accumulative time counter (seconds)

    void Update() {
        ++framesCounter;
        currentTime = GetTime();
        updateDrawTime = currentTime - previousTime;

        if (Config::GetInt("fps") > 0)          // We want a fixed frame rate
        {
            waitTime = (1.0f/static_cast<float>(Config::GetInt("fps"))) - updateDrawTime;
            if (waitTime > 0.0)
            {
                WaitTime(static_cast<float>(waitTime));
                currentTime = GetTime();
                deltaTime = static_cast<float>(currentTime - previousTime);
            }
        }
        else deltaTime = static_cast<float>(updateDrawTime);    // Framerate could be variable

        previousTime = currentTime;
    }
};
#endif //APTOMODO_FRAMERATEMANAGER_H
