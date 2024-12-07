//
// Created by ginko on 12/7/24.
//

#ifndef APTOMODO_AIEDITOR_H
#define APTOMODO_AIEDITOR_H


#include "managers/game.h"
#include <rlgl.h>

LevelOutcome PlayAIEditor();

class Widget {
public:
    Vector2 position;

    virtual ~Widget() = default;

    explicit Widget(Vector2 position) : position(position) {};

    virtual void draw() = 0;

    virtual void Move(Vector2 newPosition) { position = newPosition; };
};

namespace Widgets {
    class State : public Widget {
    public:
        static constexpr float radius{20};
        static constexpr Color color{BLACK};
        static constexpr Color colorSelected{VIOLET};
        static constexpr float lw{20.0f};

        void draw() override {
            float oldLW = rlGetLineWidth();
//            rlSetLineWidth(lw);
            DrawCircleLinesV(position, radius, color);
//            rlSetLineWidth(oldLW);

        }

        void drawSelected() {
            DrawCircleLinesV(position, radius, colorSelected);
        }


        void swap(Widgets::State &other) {
            // Swap the mutable data
            std::swap(position, other.position);
        }

        explicit State(Vector2 position) : Widget(position) {

        }
    };
}

class View;

class State {
public:
    virtual ~State() = default;

    virtual void handleInput(View &view) = 0;

    virtual void enter(View &view) = 0;
};


class Default : public State {
    void handleInput(View &view) override;

public:
    void enter(View &view) override;
};

class MovingView : public State {
    Vector2 start;

    void handleInput(View &view) override;

public:
    void enter(View &view) override;
};

class HoveringState : public State {

    void handleInput(View &view) override;

public:
    void enter(View &view) override;
};


class View {
public:
    static Default defaultState;
    static MovingView movingView;
    static HoveringState hoveringState;
    State *state = &defaultState;

    void handleInput();
};


#endif //APTOMODO_AIEDITOR_H
