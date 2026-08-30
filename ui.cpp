#include "ui.hpp"

void ui::drawTopBanner(){
    
    int sHeight = GetScreenHeight(), sWidth = GetScreenWidth();

    DrawRectangle(0, 0, sWidth, sHeight / 6, Color{ 0, 80, 170, 125});


}

void ui::drawButtons(int waterCooldown, int retardantCooldown){

    int sHeight = GetScreenHeight(), sWidth = GetScreenWidth();
    int bannerHeight = sHeight / 6;
    const char* waterText = TextFormat("Water cooldown: %d", waterCooldown);
    const char* retardantText = TextFormat("Retardant cooldown: %d", retardantCooldown);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
    GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(Color{255, 255, 255, 80}));
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, ColorToInt(Color{255, 255, 255, 110}));
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, ColorToInt(Color{255, 255, 255, 140}));

    DrawText(waterText, 10, 8, 24, BLUE);
    DrawText(retardantText, sWidth - MeasureText(retardantText, 24) - 10, 8, 24, BLUE);

    if(GuiButton(Rectangle{10.0f, bannerHeight / 2.0f, sWidth / 2.0f - 15.0f, bannerHeight / 2.0f - 10.0f}, "Water (One)") || IsKeyPressed(KEY_ONE)){
        setLiquidType(waterT);
    }

    if(GuiButton(Rectangle{sWidth / 2.0f + 5.0f, bannerHeight / 2.0f, sWidth / 2.0f - 15.0f, bannerHeight / 2.0f - 10.0f}, "Retardant (Two)") || IsKeyPressed(KEY_TWO)){
        setLiquidType(retardantT);
    }
}
