#include "ui.hpp"

void ui::topBanner(){
    
    int sHeight = GetScreenHeight(), sWidth = GetScreenWidth();

    DrawRectangle(0, 0, sWidth, sHeight / 6, Color{ 0, 80, 170, 125});


}