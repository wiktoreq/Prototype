#include "ActuatorMenu.h"

ActuatorMenu::ActuatorMenu(TFT_eSPI* displayInstance)
    : display(displayInstance),
      speedChangedHandler(nullptr),
      directionChangedHandler(nullptr),
      activeTouchTarget(TouchTarget::None)
{
}

void ActuatorMenu::setEventHandlers(SpeedEventHandler speedChanged,
                                    DirectionEventHandler directionChanged)
{
    speedChangedHandler = speedChanged;
    directionChangedHandler = directionChanged;
}

void ActuatorMenu::configureSpeedSlider()
{
    int16_t cardWidth = TFT_W - (PAD * 2);

    speedSlider.cardX = PAD;
    speedSlider.cardY = CONTENT_Y + PAD;
    speedSlider.cardW = cardWidth;
    speedSlider.cardH = ACT_SLIDER_CARD_H;
    speedSlider.trackX = PAD + 20;
    speedSlider.trackY = speedSlider.cardY + 58;
    speedSlider.trackW = cardWidth - 40;
    speedSlider.trackH = SLIDER_TRACK_H;
    speedSlider.knobRadius = SLIDER_KNOB_R;
    speedSlider.value = 50;
    speedSlider.oldValue = 50;
    speedSlider.label = "SPEED";
    speedSlider.labelX = PAD + 16;
    speedSlider.fillColor = COLOR_ACCENT;
}

void ActuatorMenu::configureButtons()
{
    contractButton.x = PAD;
    contractButton.y = ACT_BTN_Y;
    contractButton.w = ACT_BTN_W;
    contractButton.h = ACT_BTN_H;
    contractButton.label = "CONTRACT";
    contractButton.color = COLOR_BTN_CONTRACT;
    contractButton.pressedColor = COLOR_BTN_CONTRACT_PRESSED;
    contractButton.icon = ICON_DOWN;
    contractButton.pressed = false;

    retractButton.x = PAD + ACT_BTN_W + GAP;
    retractButton.y = ACT_BTN_Y;
    retractButton.w = ACT_BTN_W;
    retractButton.h = ACT_BTN_H;
    retractButton.label = "RETRACT";
    retractButton.color = COLOR_BTN_RETRACT;
    retractButton.pressedColor = COLOR_BTN_RETRACT_PRESSED;
    retractButton.icon = ICON_UP;
    retractButton.pressed = false;
}

void ActuatorMenu::init()
{
    configureSpeedSlider();
    configureButtons();
    activeTouchTarget = TouchTarget::None;
}

void ActuatorMenu::drawSpeedControl()
{
    UiWidgets::drawCard(display,
                        speedSlider.cardX,
                        speedSlider.cardY,
                        speedSlider.cardW,
                        speedSlider.cardH);
    UiWidgets::drawSliderLabel(display, speedSlider);
    UiWidgets::drawStaticTrack(display, speedSlider);
    UiWidgets::pushKnob(speedSlider);
    UiWidgets::pushValue(speedSlider);
}

void ActuatorMenu::drawButtons()
{
    contractButton.pressed = false;
    retractButton.pressed = false;
    UiWidgets::pushButton(contractButton);
    UiWidgets::pushButton(retractButton);
}

void ActuatorMenu::draw()
{
    UiWidgets::clearContent(display);
    drawSpeedControl();
    drawButtons();
}

void ActuatorMenu::notifyDirectionChanged()
{
    if (directionChangedHandler != nullptr) {
        directionChangedHandler(contractButton.pressed, retractButton.pressed);
    }
}

void ActuatorMenu::updateButtonPressedState(TouchButton& button, bool pressed)
{
    if (button.pressed == pressed) {
        return;
    }

    button.pressed = pressed;
    UiWidgets::pushButton(button);
    notifyDirectionChanged();
}

void ActuatorMenu::selectTouchTarget(int16_t touchX, int16_t touchY)
{
    bool speedSliderWasTouched = UiWidgets::hitTest(touchX,
                                                    touchY,
                                                    speedSlider.cardX,
                                                    speedSlider.cardY,
                                                    speedSlider.cardW,
                                                    speedSlider.cardH);
    if (speedSliderWasTouched) {
        activeTouchTarget = TouchTarget::SpeedSlider;
        return;
    }

    if (UiWidgets::buttonContains(contractButton, touchX, touchY)) {
        activeTouchTarget = TouchTarget::ContractButton;
        updateButtonPressedState(contractButton, true);
        return;
    }

    if (UiWidgets::buttonContains(retractButton, touchX, touchY)) {
        activeTouchTarget = TouchTarget::RetractButton;
        updateButtonPressedState(retractButton, true);
    }
}

void ActuatorMenu::handleSpeedSliderTouch(int16_t touchX)
{
    bool valueChanged = UiWidgets::handleSliderTouch(speedSlider, touchX);
    if (!valueChanged) {
        return;
    }

    UiWidgets::updateSlider(speedSlider);
    UiWidgets::pushValue(speedSlider);

    if (speedChangedHandler != nullptr) {
        speedChangedHandler(speedSlider.value);
    }
}

void ActuatorMenu::handleTouch(int16_t touchX, int16_t touchY)
{
    if (activeTouchTarget == TouchTarget::None) {
        selectTouchTarget(touchX, touchY);
    }

    if (activeTouchTarget == TouchTarget::SpeedSlider) {
        handleSpeedSliderTouch(touchX);
        return;
    }

    if (activeTouchTarget == TouchTarget::ContractButton) {
        bool isInsideButton = UiWidgets::buttonContains(contractButton, touchX, touchY);
        updateButtonPressedState(contractButton, isInsideButton);
        updateButtonPressedState(retractButton, false);
        return;
    }

    if (activeTouchTarget == TouchTarget::RetractButton) {
        bool isInsideButton = UiWidgets::buttonContains(retractButton, touchX, touchY);
        updateButtonPressedState(retractButton, isInsideButton);
        updateButtonPressedState(contractButton, false);
    }
}

void ActuatorMenu::handleRelease()
{
    updateButtonPressedState(contractButton, false);
    updateButtonPressedState(retractButton, false);
    activeTouchTarget = TouchTarget::None;
}

uint8_t ActuatorMenu::getSpeed() const
{
    return speedSlider.value;
}

bool ActuatorMenu::isContractPressed() const
{
    return contractButton.pressed;
}

bool ActuatorMenu::isRetractPressed() const
{
    return retractButton.pressed;
}
