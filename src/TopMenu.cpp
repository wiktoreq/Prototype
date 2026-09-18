#include "TopMenu.h"

#include "Config.h"
#include "consola28.h"

TopMenu::TopMenu(TFT_eSPI* displayInstance)
    : display(displayInstance),
      activeTabIndex(0),
      tabWidth(0)
{
}

void TopMenu::addTab(const std::string& name)
{
    tabNames.push_back(name);
}

void TopMenu::init()
{
    if (tabNames.empty()) {
        tabWidth = 0;
        return;
    }

    tabWidth = TFT_W / static_cast<int>(tabNames.size());
}

void TopMenu::draw()
{
    display->loadFont(consola28);
    display->setTextDatum(MC_DATUM);

    for (std::size_t index = 0; index < tabNames.size(); index++) {
        int tabIndex = static_cast<int>(index);
        int16_t tabX = tabIndex * tabWidth;
        bool tabIsActive = tabIndex == activeTabIndex;

        uint16_t tabColor = COLOR_TAB_IDLE;
        uint16_t labelColor = COLOR_TEXT_DIM;
        if (tabIsActive) {
            tabColor = COLOR_TAB_ACTIVE;
            labelColor = COLOR_TEXT;
        }

        display->fillRect(tabX, 0, tabWidth, MENU_HEIGHT, tabColor);

        if (tabIndex > 0) {
            display->drawFastVLine(tabX, 8, MENU_HEIGHT - 16, COLOR_TRACK);
        }

        display->setTextColor(labelColor, tabColor);
        display->drawString(tabNames[index].c_str(),
                            tabX + (tabWidth / 2),
                            (MENU_HEIGHT / 2) - 2);
    }

    display->drawFastHLine(0, MENU_HEIGHT, TFT_W, COLOR_ACCENT);

    int16_t activeTabX = activeTabIndex * tabWidth;
    display->fillRect(activeTabX + 12,
                      MENU_HEIGHT - 3,
                      tabWidth - 24,
                      3,
                      COLOR_ACCENT);
    display->unloadFont();
}

bool TopMenu::touchIsInsideMenu(int16_t y) const
{
    return y <= MENU_HEIGHT && tabWidth > 0;
}

int TopMenu::constrainTabIndex(int tabIndex) const
{
    if (tabIndex < 0) {
        return 0;
    }

    int lastTabIndex = static_cast<int>(tabNames.size()) - 1;
    if (tabIndex > lastTabIndex) {
        return lastTabIndex;
    }

    return tabIndex;
}

int TopMenu::handleTouch(int16_t x, int16_t y)
{
    if (!touchIsInsideMenu(y)) {
        return -1;
    }

    int tappedIndex = constrainTabIndex(x / tabWidth);
    if (tappedIndex == activeTabIndex) {
        return -1;
    }

    activeTabIndex = tappedIndex;
    draw();
    return activeTabIndex;
}

int TopMenu::getActiveTab() const
{
    return activeTabIndex;
}
