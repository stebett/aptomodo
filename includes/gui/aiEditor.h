//
// Created by ginko on 12/7/24.
//

#ifndef APTOMODO_AIEDITOR_H
#define APTOMODO_AIEDITOR_H


#include "managers/game.h"

LevelOutcome PlayAIEditor();

class View;

class State {
public:

    virtual ~State() = default;
    virtual void handleInput(View& view) = 0;
    virtual void onEnter(View& view) = 0;
};


class Default: public State {
    void handleInput(View& view) override;
    void onEnter(View& view) override;

};

class MovingView: public State {
    Vector2 start;
    void handleInput(View& view) override;
    void onEnter(View& view) override;
};


class View {
public:
    static Default defaultState;
    static MovingView movingView;
    State* state = &defaultState;

    void handleInput();
};


#endif //APTOMODO_AIEDITOR_H
