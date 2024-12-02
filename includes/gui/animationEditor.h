//
// Created by ginko on 12/2/24.

//Animation system
//in editor drag and drop of animations
//allows to define rectangles
//it saves them in resources with the png + a toml file that describes starts and ends of bounding boxes, timings etc.
//Attach them to a state of an entity


#ifndef APTOMODO_ANIMATIONEDITOR_H
#define APTOMODO_ANIMATIONEDITOR_H

#include "managers/game.h"

namespace ImGui {

    void ShowAnimationEditor();

}
LevelOutcome AnimationEditorLevel(const Camera2D &camera);

LevelOutcome PlayAnimationEditorLevel();


#endif //APTOMODO_ANIMATIONEDITOR_H
