#include "MainMenu.h"

MainMenu::MainMenu(TFT_eSPI* displayInstance)
    : display(displayInstance),
      brightnessChangedHandler(nullptr),
      ledPowerHandler(nullptr),
      muteHandler(nullptr),
      activeTouchTarget(TouchTarget::None)
{
}

void MainMenu::setEventHandlers(SliderEventHandler brightnessChanged,
                                ButtonEventHandler ledPowerPressed,
                                ButtonEventHandler mutePressed)
{
    brightnessChangedHandler = brightnessChanged;
    ledPowerHandler = ledPowerPressed;
    muteHandler = mutePressed;
}

void MainMenu::configureLightSlider()
{
    int16_t cardWidth = TFT_W - (PAD * 2);

    lightSlider.cardX = PAD;
    lightSlider.cardY = CONTENT_Y + PAD;
    lightSlider.cardW = cardWidth;
    lightSlider.cardH = MAIN_CARD_H;
    lightSlider.trackX = PAD + 20;
    lightSlider.trackY = lightSlider.cardY + 64;
    lightSlider.trackW = cardWidth - 40;
    lightSlider.trackH = SLIDER_TRACK_H;
    lightSlider.knobRadius = SLIDER_KNOB_R;
    lightSlider.value = 40;
    lightSlider.oldValue = 40;
    lightSlider.label = "LIGHT";
    lightSlider.labelX = PAD + 44;
    lightSlider.fillColor = COLOR_LIGHT_FILL;
}

void MainMenu::configureButtons()
{
    ledPowerButton.x = PAD;
    ledPowerButton.y = MAIN_BTN_Y;
    ledPowerButton.w = MAIN_BTN_W;
    ledPowerButton.h = MAIN_BTN_H;
    ledPowerButton.label = "LED ON/OFF";
    ledPowerButton.color = COLOR_BTN_CONTRACT;
    ledPowerButton.pressedColor = COLOR_BTN_CONTRACT_PRESSED;
    ledPowerButton.icon = ICON_NONE;
    ledPowerButton.pressed = false;

    muteButton.x = PAD + MAIN_BTN_W + GAP;
    muteButton.y = MAIN_BTN_Y;
    muteButton.w = MAIN_BTN_W;
    muteButton.h = MAIN_BTN_H;
    muteButton.label = "MUTE";
    muteButton.color = COLOR_BTN_RETRACT;
    muteButton.pressedColor = COLOR_BTN_RETRACT_PRESSED;
    muteButton.icon = ICON_NONE;
    muteButton.pressed = false;
}

void MainMenu::init()
{
    configureLightSlider();
    configureButtons();
    activeTouchTarget = TouchTarget::None;
}

void MainMenu::drawLightControl()
{
    UiWidgets::drawCard(display,
                        lightSlider.cardX,
                        lightSlider.cardY,
                        lightSlider.cardW,
                        lightSlider.cardH);

    int16_t iconX = lightSlider.cardX + 24;
    int16_t iconY = lightSlider.cardY + 24;
    UiWidgets::drawLampIcon(display, iconX, iconY, COLOR_LIGHT_FILL);

    UiWidgets::drawSliderLabel(display, lightSlider);
    UiWidgets::drawStaticTrack(display, lightSlider);
    UiWidgets::pushKnob(lightSlider);
    UiWidgets::pushValue(lightSlider);
}

void MainMenu::drawButtons()
{
    ledPowerButton.pressed = false;
    muteButton.pressed = false;
    UiWidgets::pushButton(ledPowerButton);
    UiWidgets::pushButton(muteButton);
}

void MainMenu::draw()
{
    UiWidgets::clearContent(display);
    drawLightControl();
    drawButtons();
}

void MainMenu::updateButtonPressedState(TouchButton& button, bool pressed)
{
    if (button.pressed == pressed) {
        return;
    }

    button.pressed = pressed;
    UiWidgets::pushButton(button);
}

void MainMenu::selectTouchTarget(int16_t touchX, int16_t touchY)
{
    bool lightSliderWasTouched = UiWidgets::hitTest(touchX,
                                                    touchY,
                                                    lightSlider.cardX,
                                                    lightSlider.cardY,
                                                    lightSlider.cardW,
                                                    lightSlider.cardH);
    if (lightSliderWasTouched) {
        activeTouchTarget = TouchTarget::LightSlider;
        return;
    }

    if (UiWidgets::buttonContains(ledPowerButton, touchX, touchY)) {
        activeTouchTarget = TouchTarget::LedPowerButton;
        updateButtonPressedState(ledPowerButton, true);
        if (ledPowerHandler != nullptr) {
            ledPowerHandler();
        }
        return;
    }

    if (UiWidgets::buttonContains(muteButton, touchX, touchY)) {
        activeTouchTarget = TouchTarget::MuteButton;
        updateButtonPressedState(muteButton, true);
        if (muteHandler != nullptr) {
            muteHandler();
        }
    }
}

void MainMenu::handleLightSliderTouch(int16_t touchX)
{
    bool valueChanged = UiWidgets::handleSliderTouch(lightSlider, touchX);
    if (!valueChanged) {
        return;
    }

    UiWidgets::updateSlider(lightSlider);
    UiWidgets::pushValue(lightSlider);

    if (brightnessChangedHandler != nullptr) {
        brightnessChangedHandler(lightSlider.value);
    }
}

void MainMenu::handleTouch(int16_t touchX, int16_t touchY)
{
    if (activeTouchTarget == TouchTarget::None) {
        selectTouchTarget(touchX, touchY);
    }

    if (activeTouchTarget == TouchTarget::LightSlider) {
        handleLightSliderTouch(touchX);
        return;
    }

    if (activeTouchTarget == TouchTarget::LedPowerButton) {
        bool isInsideButton = UiWidgets::buttonContains(ledPowerButton, touchX, touchY);
        updateButtonPressedState(ledPowerButton, isInsideButton);
        return;
    }

    if (activeTouchTarget == TouchTarget::MuteButton) {
        bool isInsideButton = UiWidgets::buttonContains(muteButton, touchX, touchY);
        updateButtonPressedState(muteButton, isInsideButton);
    }
}

void MainMenu::handleRelease()
{
    updateButtonPressedState(ledPowerButton, false);
    updateButtonPressedState(muteButton, false);
    activeTouchTarget = TouchTarget::None;
}

uint8_t MainMenu::getLightBrightness() const
{
    return lightSlider.value;
}
