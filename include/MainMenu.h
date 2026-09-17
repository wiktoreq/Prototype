#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Screen.h"
#include "UiWidgets.h"

class MainMenu : public Screen {
public:
    using SliderEventHandler = void (*)(uint8_t value);
    using ButtonEventHandler = void (*)();

private:
    TFT_eSPI* tft;
    HorizontalSlider lightSlider;
    TouchButton sleepBtn;
    TouchButton muteBtn;
    SliderEventHandler brightnessChangedHandler;
    ButtonEventHandler sleepHandler;
    ButtonEventHandler muteHandler;

    enum DragTarget {
        DRAG_NONE = 0,
        DRAG_LIGHT = 1,
        DRAG_SLEEP = 2,
        DRAG_MUTE = 3
    };
    DragTarget dragTarget;

    // Fills the external-light slider and button geometry.
    void layoutControls();

    // Paints the external-light card's static chrome.
    void drawBrightnessChrome();

    // Redraws a button only when its pressed state changes.
    void setButtonPressed(TouchButton& button, bool pressed);

public:
    // Binds this menu to the shared TFT driver.
    MainMenu(TFT_eSPI* tftInstance);

    // Registers hardware event handlers for all main-menu components.
    void setEventHandlers(SliderEventHandler onBrightnessChanged,
                          ButtonEventHandler onSleep,
                          ButtonEventHandler onMute);

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
