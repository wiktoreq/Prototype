#include "MainMenu.h"

// Binds this menu to the shared TFT driver.
MainMenu::MainMenu(TFT_eSPI* tftInstance)
    : tft(tftInstance),
      brightnessChangedHandler(nullptr),
      sleepHandler(nullptr),
      muteHandler(nullptr),
      dragTarget(DRAG_NONE) {}

// Registers hardware event handlers for all main-menu components.
void MainMenu::setEventHandlers(SliderEventHandler onBrightnessChanged,
                                ButtonEventHandler onSleep,
                                ButtonEventHandler onMute) {
    brightnessChangedHandler = onBrightnessChanged;
    sleepHandler = onSleep;
    muteHandler = onMute;
}

// Fills the external-light slider and button geometry.
void MainMenu::layoutControls() {
    const int16_t cardW = TFT_W - (PAD * 2);
    const int16_t trackW = cardW - 40;
    const int16_t trackH = SLIDER_TRACK_H;

    lightSlider.cardX = PAD;
    lightSlider.cardY = CONTENT_Y + PAD;
    lightSlider.cardW = cardW;
    lightSlider.cardH = MAIN_CARD_H;
    lightSlider.trackX = PAD + 20;
    lightSlider.trackY = lightSlider.cardY + 64;
    lightSlider.trackW = trackW;
    lightSlider.trackH = trackH;
    lightSlider.knobRadius = SLIDER_KNOB_R;
    lightSlider.label = "LIGHT";
    lightSlider.labelX = PAD + 44;
    lightSlider.fillColor = COLOR_LIGHT_FILL;

    sleepBtn.x = PAD;
    sleepBtn.y = MAIN_BTN_Y;
    sleepBtn.w = MAIN_BTN_W;
    sleepBtn.h = MAIN_BTN_H;
    sleepBtn.label = "SLEEP";
    sleepBtn.color = COLOR_BTN_CONTRACT;
    sleepBtn.pressedColor = COLOR_BTN_CONTRACT_PRESSED;
    sleepBtn.icon = ICON_NONE;
    sleepBtn.pressed = false;

    muteBtn.x = PAD + MAIN_BTN_W + GAP;
    muteBtn.y = MAIN_BTN_Y;
    muteBtn.w = MAIN_BTN_W;
    muteBtn.h = MAIN_BTN_H;
    muteBtn.label = "MUTE";
    muteBtn.color = COLOR_BTN_RETRACT;
    muteBtn.pressedColor = COLOR_BTN_RETRACT_PRESSED;
    muteBtn.icon = ICON_NONE;
    muteBtn.pressed = false;
}

// Paints the external-light card's static chrome.
void MainMenu::drawBrightnessChrome() {
    UiWidgets::drawCard(tft, lightSlider.cardX, lightSlider.cardY,
                        lightSlider.cardW, lightSlider.cardH);
    UiWidgets::drawLampIcon(tft, lightSlider.cardX + 24, lightSlider.cardY + 24,
                            COLOR_LIGHT_FILL);

    UiWidgets::drawSliderLabel(tft, lightSlider);
    UiWidgets::drawStaticTrack(tft, lightSlider);
}

// Redraws a button only when its pressed state changes.
void MainMenu::setButtonPressed(TouchButton& button, bool pressed) {
    if (button.pressed == pressed) {
        return;
    }
    button.pressed = pressed;
    UiWidgets::pushButton(button);
}

// Sets the default external-light value and card layout.
void MainMenu::init() {
    layoutControls();
    lightSlider.value = 40;
    lightSlider.oldValue = 40;
    dragTarget = DRAG_NONE;
}

// Draws the external-light card into the content area.
void MainMenu::draw() {
    UiWidgets::clearContent(tft);
    drawBrightnessChrome();
    UiWidgets::pushKnob(lightSlider);
    UiWidgets::pushValue(lightSlider);
    sleepBtn.pressed = false;
    muteBtn.pressed = false;
    UiWidgets::pushButton(sleepBtn);
    UiWidgets::pushButton(muteBtn);
}

// Drags the external-light slider from a content-area touch.
void MainMenu::handleTouch(int16_t touchX, int16_t touchY) {
    if (dragTarget == DRAG_NONE) {
        if (UiWidgets::hitTest(touchX, touchY, lightSlider.cardX, lightSlider.cardY,
                               lightSlider.cardW, lightSlider.cardH)) {
            dragTarget = DRAG_LIGHT;
        } else if (UiWidgets::buttonContains(sleepBtn, touchX, touchY)) {
            dragTarget = DRAG_SLEEP;
            setButtonPressed(sleepBtn, true);
            if (sleepHandler != nullptr) {
                sleepHandler();
            }
        } else if (UiWidgets::buttonContains(muteBtn, touchX, touchY)) {
            dragTarget = DRAG_MUTE;
            setButtonPressed(muteBtn, true);
            if (muteHandler != nullptr) {
                muteHandler();
            }
        } else {
            return;
        }
    }

    if (dragTarget == DRAG_LIGHT &&
        UiWidgets::handleSliderTouch(lightSlider, touchX)) {
        UiWidgets::updateSlider(lightSlider);
        UiWidgets::pushValue(lightSlider);
        if (brightnessChangedHandler != nullptr) {
            brightnessChangedHandler(lightSlider.value);
        }
    } else if (dragTarget == DRAG_SLEEP) {
        setButtonPressed(sleepBtn, UiWidgets::buttonContains(sleepBtn, touchX, touchY));
    } else if (dragTarget == DRAG_MUTE) {
        setButtonPressed(muteBtn, UiWidgets::buttonContains(muteBtn, touchX, touchY));
    }
}

// Clears the active slider drag when the finger lifts.
void MainMenu::handleRelease() {
    setButtonPressed(sleepBtn, false);
    setButtonPressed(muteBtn, false);
    dragTarget = DRAG_NONE;
}
