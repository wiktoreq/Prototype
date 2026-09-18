#pragma once

#include <string>
#include <vector>

#include <TFT_eSPI.h>

class TopMenu
{
public:
    explicit TopMenu(TFT_eSPI* display);

    void addTab(const std::string& name);
    void init();
    void draw();

    // Selects a touched tab and returns its index, or -1 when nothing changed.
    int handleTouch(int16_t x, int16_t y);

    int getActiveTab() const;

private:
    TFT_eSPI* display;
    std::vector<std::string> tabNames;
    int activeTabIndex;
    int tabWidth;

    bool touchIsInsideMenu(int16_t y) const;
    int constrainTabIndex(int tabIndex) const;
};
