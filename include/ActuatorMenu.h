#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "Screen.h"
#include "UiWidgets.h"

class ActuatorMenu : public Screen
{
public:
    using SpeedEventHandler = void (*)(uint8_t value);
    using DirectionEventHandler = void (*)(bool contracting, bool retracting);

    explicit ActuatorMenu(TFT_eSPI* display);

    void setEventHandlers(SpeedEventHandler speedChanged,
                          DirectionEventHandler directionChanged);

    void init() override;
    void draw() override;
    void handleTouch(int16_t touchX, int16_t touchY) override;
    void handleRelease() override;

    uint8_t getSpeed() const;
    bool isContractPressed() const;
    bool isRetractPressed() const;

private:
    enum class TouchTarget : uint8_t
    {
        None,
        SpeedSlider,
        ContractButton,
        RetractButton
    };

    TFT_eSPI* display;
    HorizontalSlider speedSlider;
    TouchButton contractButton;
    TouchButton retractButton;
    SpeedEventHandler speedChangedHandler;
    DirectionEventHandler directionChangedHandler;
    TouchTarget activeTouchTarget;

    void configureSpeedSlider();
    void configureButtons();
    void drawSpeedControl();
    void drawButtons();
    void selectTouchTarget(int16_t touchX, int16_t touchY);
    void handleSpeedSliderTouch(int16_t touchX);
    void updateButtonPressedState(TouchButton& button, bool pressed);
    void notifyDirectionChanged();
};
