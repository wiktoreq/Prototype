#include "DisplayApp.h"

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include "FS.h"
#include "SPIFFS.h"

#include "ActuatorMenu.h"
#include "Config.h"
#include "HardwareController.h"
#include "MainMenu.h"
#include "Screen.h"
#include "SystemPower.h"
#include "TopMenu.h"
#include "UiWidgets.h"

namespace
{
constexpr char TOUCH_CALIBRATION_FILE[] = "/TouchCalData";
constexpr bool FORCE_TOUCH_CALIBRATION = true;

enum class AppScreen : uint8_t
{
    Main,
    Height,
    Position
};

TFT_eSPI display;
TopMenu topMenu(&display);
MainMenu mainMenu(&display);
ActuatorMenu heightMenu(&display);
ActuatorMenu positionMenu(&display);
Screen* activeScreen = &mainMenu;
bool touchWasActive = false;

void configureUiEventHandlers()
{
    SystemPower::setDisplayPowerActions(DisplayApp::sleepOn, DisplayApp::sleepOff);

    mainMenu.setEventHandlers(HardwareController::setLightBrightness,
                              HardwareController::toggleLed,
                              HardwareController::toggleMute);

    heightMenu.setEventHandlers(HardwareController::setHeightSpeed,
                                HardwareController::setHeightDirection);

    positionMenu.setEventHandlers(HardwareController::setPositionSpeed,
                                  HardwareController::setPositionDirection);
}

void switchToScreen(AppScreen newScreen)
{
    if (newScreen == AppScreen::Height) {
        activeScreen = &heightMenu;
    } else if (newScreen == AppScreen::Position) {
        activeScreen = &positionMenu;
    } else {
        activeScreen = &mainMenu;
    }

    activeScreen->draw();
}

void mountFileSystem()
{
    if (SPIFFS.begin()) {
        return;
    }

    Serial.println("Formatting touch calibration file system");
    SPIFFS.format();
    SPIFFS.begin();
}

bool loadTouchCalibration(uint16_t* calibrationData, size_t dataSize)
{
    if (!SPIFFS.exists(TOUCH_CALIBRATION_FILE)) {
        return false;
    }

    if (FORCE_TOUCH_CALIBRATION) {
        SPIFFS.remove(TOUCH_CALIBRATION_FILE);
        return false;
    }

    fs::File calibrationFile = SPIFFS.open(TOUCH_CALIBRATION_FILE, "r");
    if (!calibrationFile) {
        return false;
    }

    size_t bytesRead = calibrationFile.readBytes(
        reinterpret_cast<char*>(calibrationData),
        dataSize);
    calibrationFile.close();
    return bytesRead == dataSize;
}

void saveTouchCalibration(const uint16_t* calibrationData, size_t dataSize)
{
    fs::File calibrationFile = SPIFFS.open(TOUCH_CALIBRATION_FILE, "w");
    if (!calibrationFile) {
        return;
    }

    calibrationFile.write(
        reinterpret_cast<const unsigned char*>(calibrationData),
        dataSize);
    calibrationFile.close();
}

void showCalibrationInstructions()
{
    display.fillScreen(TFT_BLACK);
    display.setCursor(20, 0);
    display.setTextFont(2);
    display.setTextSize(1);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.println("Touch corners as indicated");

    if (FORCE_TOUCH_CALIBRATION) {
        display.setTextColor(TFT_RED, TFT_BLACK);
        display.println("Calibration will run again after restart");
    }
}

void configureTouchCalibration()
{
    uint16_t calibrationData[5] = {};
    size_t calibrationDataSize = sizeof(calibrationData);

    mountFileSystem();
    if (loadTouchCalibration(calibrationData, calibrationDataSize)) {
        display.setTouch(calibrationData);
        return;
    }

    showCalibrationInstructions();
    display.calibrateTouch(calibrationData, TFT_MAGENTA, TFT_BLACK, 15);
    saveTouchCalibration(calibrationData, calibrationDataSize);
}

void configureMenus()
{
    topMenu.addTab("Main");
    topMenu.addTab("Height");
    topMenu.addTab("Position");
    topMenu.init();
    topMenu.draw();

    mainMenu.init();
    heightMenu.init();
    positionMenu.init();
    switchToScreen(AppScreen::Main);
}

void handleMenuTouch(uint16_t touchX, uint16_t touchY)
{
    int selectedTab = topMenu.handleTouch(touchX, touchY);
    if (selectedTab < 0) {
        return;
    }

    activeScreen->handleRelease();
    switchToScreen(static_cast<AppScreen>(selectedTab));
}
}

void DisplayApp::setup()
{
    display.init();
    display.setRotation(3);
    display.fillScreen(COLOR_BG);

    configureUiEventHandlers();
    UiWidgets::init(&display);
    configureTouchCalibration();
    display.fillScreen(COLOR_BG);
    configureMenus();
}

void DisplayApp::loop()
{
    uint16_t touchX = 0;
    uint16_t touchY = 0;
    bool screenIsTouched = display.getTouch(&touchX, &touchY);

    if (screenIsTouched) {
        if (touchY <= MENU_HEIGHT) {
            handleMenuTouch(touchX, touchY);
        } else {
            activeScreen->handleTouch(touchX, touchY);
        }
        touchWasActive = true;
        return;
    }

    if (touchWasActive) {
        activeScreen->handleRelease();
        touchWasActive = false;
    }
}

UiControlState DisplayApp::getControlState()
{
    UiControlState state = {};
    state.lightBrightness = mainMenu.getLightBrightness();
    state.heightSpeed = heightMenu.getSpeed();
    state.positionSpeed = positionMenu.getSpeed();
    return state;
}

void DisplayApp::sleepOn()
{
    display.writecommand(TFT_DISPOFF);
    display.writecommand(TFT_SLPIN);
}

void DisplayApp::sleepOff()
{
    display.writecommand(TFT_DISPON);
    display.writecommand(TFT_SLPOUT);
}
