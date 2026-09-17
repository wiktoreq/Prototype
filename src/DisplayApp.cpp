#include "DisplayApp.h"

#define CALIBRATION_FILE "/TouchCalData"
#define REPEAT_CAL true

static TFT_eSPI tft = TFT_eSPI();
static TopMenu topMenu(&tft);
static MainMenu mainMenu(&tft);
static ActuatorMenu heightMenu(&tft);
static ActuatorMenu positionMenu(&tft);

enum AppState {
    STATE_MAIN = 0,
    STATE_HEIGHT = 1,
    STATE_POSITION = 2
};

static AppState currentState = STATE_MAIN;
static Screen* currentScreen = &mainMenu;
static bool touchWasActive = false;

static void switchToState(AppState newState);
static void touch_calibrate();

// Maps a tab index to its content screen and draws that screen.
static void switchToState(AppState newState) {
    currentState = newState;
    switch (currentState) {
        case STATE_HEIGHT:
            currentScreen = &heightMenu;
            break;
        case STATE_POSITION:
            currentScreen = &positionMenu;
            break;
        case STATE_MAIN:
        default:
            currentScreen = &mainMenu;
            break;
    }
    currentScreen->draw();
}

// Loads stored touch calibration or runs the on-screen calibration wizard.
static void touch_calibrate() {
    uint16_t calData[5];
    uint8_t calDataOK = 0;

    if (!SPIFFS.begin()) {
        Serial.println("formatting file system");
        SPIFFS.format();
        SPIFFS.begin();
    }

    if (SPIFFS.exists(CALIBRATION_FILE)) {
        if (REPEAT_CAL) {
            SPIFFS.remove(CALIBRATION_FILE);
        } else {
            fs::File f = SPIFFS.open(CALIBRATION_FILE, "r");
            if (f) {
                if (f.readBytes((char*)calData, 14) == 14) {
                    calDataOK = 1;
                }
                f.close();
            }
        }
    }

    if (calDataOK && !REPEAT_CAL) {
        tft.setTouch(calData);
        return;
    }

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Touch corners as indicated");
    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
        f.write((const unsigned char*)calData, 14);
        f.close();
    }
}

void DisplayApp::setup() {
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(COLOR_BG);

    UiWidgets::init(&tft);

    if (REPEAT_CAL) {
        touch_calibrate();
        tft.fillScreen(COLOR_BG);
    }

    topMenu.addTab("Main");
    topMenu.addTab("Height");
    topMenu.addTab("Position");
    topMenu.init();
    topMenu.draw();

    mainMenu.init();
    heightMenu.init();
    positionMenu.init();

    switchToState(STATE_MAIN);
}

void DisplayApp::loop() {
    uint16_t touchX = 0;
    uint16_t touchY = 0;
    const bool touched = tft.getTouch(&touchX, &touchY);

    if (touched) {
        if (touchY <= MENU_HEIGHT) {
            const int newTab = topMenu.handleTouch(touchX, touchY);
            if (newTab != -1) {
                currentScreen->handleRelease();
                switchToState(static_cast<AppState>(newTab));
            }
        } else {
            currentScreen->handleTouch(touchX, touchY);
        }
        touchWasActive = true;
    } else if (touchWasActive) {
        currentScreen->handleRelease();
        touchWasActive = false;
    }
}

UiControlState DisplayApp::getControlState() {
    UiControlState state;
    state.screen = static_cast<UiControlState::Screen>(currentState);
    state.lightBrightness = mainMenu.getLightBrightness();
    state.heightSpeed = heightMenu.getSpeed();
    state.heightContracting = (currentState == STATE_HEIGHT) && heightMenu.isContractPressed();
    state.heightRetracting = (currentState == STATE_HEIGHT) && heightMenu.isRetractPressed();
    state.positionSpeed = positionMenu.getSpeed();
    state.positionContracting = (currentState == STATE_POSITION) && positionMenu.isContractPressed();
    state.positionRetracting = (currentState == STATE_POSITION) && positionMenu.isRetractPressed();
    return state;
}

void DisplayApp::sleepOn() {
    tft.writecommand(TFT_DISPOFF);
    tft.writecommand(TFT_SLPIN);
}

void DisplayApp::sleepOff() {
    tft.writecommand(TFT_DISPON);
    tft.writecommand(TFT_SLPOUT);
}
