#include "MainMenu.h"

// Binds this menu to the shared TFT driver.
MainMenu::MainMenu(TFT_eSPI* tftInstance)
    : tft(tftInstance), dragTarget(DRAG_NONE) {}

// Fills the external-light slider geometry.
void MainMenu::layoutSlider() {
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

// Sets the default external-light value and card layout.
void MainMenu::init() {
    layoutSlider();
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
}

// Drags the external-light slider from a content-area touch.
void MainMenu::handleTouch(int16_t touchX, int16_t touchY) {
    if (dragTarget == DRAG_NONE) {
        if (UiWidgets::hitTest(touchX, touchY, lightSlider.cardX, lightSlider.cardY,
                               lightSlider.cardW, lightSlider.cardH)) {
            dragTarget = DRAG_LIGHT;
        } else {
            return;
        }
    }

    if (UiWidgets::handleSliderTouch(lightSlider, touchX)) {
        UiWidgets::updateSlider(lightSlider);
        UiWidgets::pushValue(lightSlider);
    }
}

// Clears the active slider drag when the finger lifts.
void MainMenu::handleRelease() {
    dragTarget = DRAG_NONE;
}
