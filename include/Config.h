#pragma once
#include <stdint.h>

// Physical display size in landscape (ILI9488 rotation 3)
constexpr int16_t TFT_W = 480;
constexpr int16_t TFT_H = 320;

// Top tab bar
constexpr int16_t MENU_HEIGHT = 44;
constexpr int16_t CONTENT_Y   = MENU_HEIGHT + 1;
constexpr int16_t CONTENT_H   = TFT_H - CONTENT_Y;

// Shared spacing and control sizes (touch-friendly on a 480x320 panel)
constexpr int16_t PAD              = 10;
constexpr int16_t GAP              = 10;
constexpr int16_t CARD_RADIUS      = 12;
constexpr int16_t BTN_RADIUS       = 12;
constexpr int16_t SLIDER_TRACK_H   = 14;
constexpr int16_t SLIDER_KNOB_R    = 16;

// Main menu: external-light slider above LED power and mute buttons
constexpr int16_t MAIN_CARD_H = 96;
constexpr int16_t MAIN_BTN_Y  = CONTENT_Y + PAD + MAIN_CARD_H + GAP;
constexpr int16_t MAIN_BTN_H  = (CONTENT_Y + CONTENT_H) - PAD - MAIN_BTN_Y;
constexpr int16_t MAIN_BTN_W  = (TFT_W - PAD * 2 - GAP) / 2;

// Actuator menus: speed card on top, two large hold-buttons below
constexpr int16_t ACT_SLIDER_CARD_H = 96;
constexpr int16_t ACT_BTN_Y         = CONTENT_Y + PAD + ACT_SLIDER_CARD_H + GAP;
constexpr int16_t ACT_BTN_H         = (CONTENT_Y + CONTENT_H) - PAD - ACT_BTN_Y;
constexpr int16_t ACT_BTN_W         = (TFT_W - PAD * 2 - GAP) / 2;

// Object-sized sprites: each buffer is only as large as the widget it paints
constexpr int16_t KNOB_SPRITE     = (SLIDER_KNOB_R * 2) + 2;
constexpr uint16_t KNOB_CHROMA    = 0xF81F; // magenta key, unused in the UI
constexpr int16_t VALUE_SPRITE_W  = 56;     // fits "100%" in font 4
constexpr int16_t VALUE_SPRITE_H  = 26;

// Monochrome RGB565 palette
constexpr uint16_t COLOR_BG = 0x0000;
constexpr uint16_t COLOR_SURFACE = 0x0000;
constexpr uint16_t COLOR_TAB_IDLE = 0x0000;
constexpr uint16_t COLOR_TAB_ACTIVE = 0x0000;
constexpr uint16_t COLOR_ACCENT = 0x0000;
constexpr uint16_t COLOR_TEXT = 0xFFFF;
constexpr uint16_t COLOR_TEXT_DIM = 0xFFFF;
constexpr uint16_t COLOR_TRACK = 0xFFFF;
constexpr uint16_t COLOR_KNOB = 0xFFFF;
constexpr uint16_t COLOR_SEPARATOR = 0x0000;
constexpr uint16_t COLOR_LIGHT_FILL = 0xFFFF;

constexpr uint16_t COLOR_BTN_CONTRACT = 0x0000;
constexpr uint16_t COLOR_BTN_CONTRACT_PRESSED = 0xFFFF;
constexpr uint16_t COLOR_BTN_RETRACT = 0x0000;
constexpr uint16_t COLOR_BTN_RETRACT_PRESSED = 0xFFFF;
constexpr uint16_t COLOR_BTN_BORDER = 0xFFFF;
