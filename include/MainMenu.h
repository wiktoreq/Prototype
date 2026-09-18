#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "Screen.h"
#include "UiWidgets.h"

class MainMenu : public Screen
{
public:
    using SliderEventHandler = void (*)(uint8_t value);
    using ButtonEventHandler = void (*)();

    explicit MainMenu(TFT_eSPI* display);

    // Connects each visible control to its hardware action.
    void setEventHandlers(SliderEventHandler brightnessChanged,
                          ButtonEventHandler ledPowerPressed,
                          ButtonEventHandler mutePressed);

    void init() override;
    void draw() override;
    void handleTouch(int16_t touchX, int16_t touchY) override;
    void handleRelease() override;

    uint8_t getLightBrightness() const;

private:
    enum class TouchTarget : uint8_t
    {
        None,
        LightSlider,
        LedPowerButton,
        MuteButton
    };

    TFT_eSPI* display;
    HorizontalSlider lightSlider;
    TouchButton ledPowerButton;
    TouchButton muteButton;
    SliderEventHandler brightnessChangedHandler;
    ButtonEventHandler ledPowerHandler;
    ButtonEventHandler muteHandler;
    TouchTarget activeTouchTarget;

    void configureLightSlider();
    void configureButtons();
    void drawLightControl();
    void drawButtons();
    void selectTouchTarget(int16_t touchX, int16_t touchY);
    void handleLightSliderTouch(int16_t touchX);
    void updateButtonPressedState(TouchButton& button, bool pressed);
};
