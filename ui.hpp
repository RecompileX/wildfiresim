#pragma once
#include "raygui\raygui.h"

enum liquidType {
    waterT,
    retardantT
};

struct ui{

    liquidType selectedLiquid = waterT;

    void drawTopBanner();
    void drawButtons(int waterCooldown, int retardantCooldown);
    liquidType getLiquidType() const { return selectedLiquid; }
    void setLiquidType(liquidType newLiquid) { selectedLiquid = newLiquid; }

};
