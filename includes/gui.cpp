//
// Created by ginko on 04/03/24.
//

#include "gui.h"

#define RAYGUI_IMPLEMENTATION

#include "raygui.h"
#include "constants.h"
#include <string>


void GUI::drawStartScreen(bool &start, bool &stop) {
    BeginDrawing();
    if (GuiButton(startButtonRec, "START")) startButton(start);
    if (GuiButton(exitButtonRec, "EXIT")) exitButton(stop);

    EndDrawing();
}

void GUI::drawGameplay() {

    fpsText = std::to_string(GetFPS()).substr(0, 2);
    mouseXText = "Mouse X: " + std::to_string(GetMouseX());
    mouseYText = "Mouse Y: " + std::to_string(GetMouseY());
    painText = std::to_string((int) progressBarValue1);
    healthText = std::to_string((int) progressBarValue2);


    DrawRectangleV({0, screenHeight - guiHeight - 2 * guiPadding},
                   Vector2{screenWidth, guiHeight + 2 * guiPadding},
                   {0, 0, 0, 150});
    GuiStatusBar(fpsRec, fpsText.c_str());
    GuiStatusBar(mouseXRec, mouseXText.c_str());
    GuiStatusBar(mouseYRec, mouseYText.c_str());
    GuiProgressBar(painRec, painText.c_str(), "Pain", &progressBarValue1, 0, progressBarMax1);
    GuiProgressBar(healthRec, healthText.c_str(), "Health", &progressBarValue2, 0, progressBarMax2);
    GuiGroupBox(skillBoxRec, "Skills");
    if (GuiButton(skillButtonRec1, "")) skillButton1();
    if (GuiButton(skillButtonRec2, "")) skillButton2();
    if (GuiButton(skillButtonRec3, "")) skillButton3();
    if (GuiButton(skillButtonRec4, "")) skillButton4();
    if (GuiButton(skillButtonRec5, "")) skillButton5();

}


void GUI::updateScreen() {
    startButtonRec = {screenWidth / 2 - guiWidthModule * 2,
                      screenHeight / 2 - guiHeight / 2,
                      guiWidthModule * 4,
                      guiHeight};

    exitButtonRec = {screenWidth / 2 - guiWidthModule * 2,
                      startButtonRec.y + startButtonRec.height + guiPadding,
                      guiWidthModule * 4,
                      guiHeight};



    fpsRec = {0 + topXPad1, 0 + guiPadding, topWidth1, guiHeight};
    mouseXRec = {0 + topXPad2, 0 + guiPadding, topWidth2, guiHeight};
    mouseYRec = {0 + topXPad3, 0 + guiPadding, topWidth3, guiHeight};

    painRec = {0 + botXPad1, screenHeight + botYPad, botWidth1 / 2, guiHeight};
    healthRec = {0 + botXPad2, screenHeight + botYPad, botWidth2 / 2, guiHeight};
    skillBoxRec = {screenWidth + botXPad3, screenHeight + botYPad, botWidth3, guiHeight};
    skillButtonRec1 = {skillBoxRec.x + boxPadding, skillBoxRec.y + boxPadding, boxSide, boxSide};
    skillButtonRec2 = {skillButtonRec1.x + 1 * (boxSide + boxPadding), skillButtonRec1.y, boxSide, boxSide};
    skillButtonRec3 = {skillButtonRec1.x + 2 * (boxSide + boxPadding), skillButtonRec1.y, boxSide, boxSide};
    skillButtonRec4 = {skillButtonRec1.x + 3 * (boxSide + boxPadding), skillButtonRec1.y, boxSide, boxSide};
    skillButtonRec5 = {skillButtonRec1.x + 4 * (boxSide + boxPadding), skillButtonRec1.y, boxSide, boxSide};
}


GUI::GUI(float screenWidth, float screenHeight, float &progressBarValue1, float &progressBarMax1,
         float &progressBarValue2, float &progressBarMax2, const char style[]) :
        screenWidth(screenWidth),
        screenHeight(screenHeight),
        progressBarValue1(progressBarValue1),
        progressBarMax1(progressBarMax1),
        progressBarValue2(progressBarValue2),
        progressBarMax2(progressBarMax2) {
    if (style) GuiLoadStyle(style);
    GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    updateScreen();
}

void GUI::startButton(bool &var) { var = true; }

void GUI::exitButton(bool &var) { var = true; }

void GUI::skillButton1() {

}

void GUI::skillButton2() {

}

void GUI::skillButton3() {

}

void GUI::skillButton4() {

}

void GUI::skillButton5() {

}
