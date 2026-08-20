#include <raylib.h>
#include <ctime>
#include "cellGrid.hpp"

cellGrid cell;
int sWidth = 640, sHeight = 640;
const int sFrameRste = 60;

int main(){

    InitWindow(sWidth, sHeight, "Wildfire Sim");
    SetTargetFPS(sFrameRste);

    while(!WindowShouldClose()){
        
        ClearBackground(BLACK);
        BeginDrawing();
        
        // Drawing
        cell.draw(sHeight, sWidth);

        // Updating
        cell.update(time(0));

        EndDrawing();

    }
    return 0;
}