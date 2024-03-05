//
// Created by ginko on 04/03/24.
//

#ifndef GUI_GUI_H
#define GUI_GUI_H

#include <string>

#include <raylib.h>



class GUI {
private:

    float screenWidth;
    float screenHeight;

    float &progressBarValue1;
    float &progressBarMax1;
    float &progressBarValue2;
    float &progressBarMax2;


    float k = {2.0f * screenHeight / 1080.0f};
    const float guiHeight{20.0f * k};
    const float guiWidthModule{30.0f * k};
    const float guiPadding{5.0f * k};
    const float boxPadding{5.0f * k};

    const float boxSide{(guiHeight - 2 * boxPadding)};

    Rectangle startButtonRec;
    Rectangle exitButtonRec;
    Rectangle fpsRec;
    Rectangle mouseXRec;
    Rectangle mouseYRec;
    Rectangle painRec;
    Rectangle healthRec;
    Rectangle skillBoxRec;
    Rectangle skillButtonRec1;
    Rectangle skillButtonRec2;
    Rectangle skillButtonRec3;
    Rectangle skillButtonRec4;
    Rectangle skillButtonRec5;

    std::string fpsText;
    std::string mouseXText;
    std::string mouseYText;
    std::string painText;
    std::string healthText;

    float topXPad1 = guiPadding;
    float topWidth1 = guiWidthModule * 2;
    float topXPad2 = guiPadding + topXPad1 + topWidth1;
    float topWidth2 = guiWidthModule * 4;
    float topXPad3 = guiPadding + topXPad2 + topWidth2;
    float topWidth3 = guiWidthModule * 4;

    float botYPad = -guiPadding - guiHeight;
    float botXPad1 = guiPadding * 6;
    float botWidth1 = guiWidthModule * 4;
    float botXPad2 = 3 * guiPadding + botXPad1 + botWidth1;
    float botWidth2 = guiWidthModule * 4;
    float botWidth3 = (boxSide + boxPadding) * 6;
    float botXPad3 = -(guiPadding + botWidth3);


public:
    GUI(float screenWidth, float screenHeight, float &progressBarValue1, float &progressBarMax1,
        float &progressBarValue2, float &progressBarMax2, const char style[] = nullptr);
    void updateScreen();
    void drawStartScreen(bool &start, bool &stop);
    void drawGameplay();
    static void startButton(bool &var);
    static void exitButton(bool &var);
    void skillButton1();
    void skillButton2();
    void skillButton3();
    void skillButton4();
    void skillButton5();
};

#endif //GUI_GUI_H
