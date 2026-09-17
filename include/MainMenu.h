#pragma once
#include <TFT_eSPI.h>
#include "Screen.h"
#include "UiWidgets.h"

class MainMenu : public Screen {
private:
    TFT_eSPI* tft;
    HorizontalSlider lightSlider;

    enum DragTarget {
        DRAG_NONE = 0,
        DRAG_LIGHT = 1
    };
    DragTarget dragTarget;

    // Fills the external-light slider geometry.
    void layoutSlider();

    // Paints the external-light card's static chrome.
    void drawBrightnessChrome();

public:
    // Binds this menu to the shared TFT driver.
    MainMenu(TFT_eSPI* tftInstance);

    // Sets the default external-light value and card layout.
    void init() override;

    // Draws the external-light card into the content area.
    void draw() override;

    // Drags the external-light slider from a content-area touch.
    void handleTouch(int16_t touchX, int16_t touchY) override;

    // Clears the active slider drag when the finger lifts.
    void handleRelease() override;

    // Returns the current external-light brightness value (0-100).
    uint8_t getLightBrightness() const { return lightSlider.value; }
};
