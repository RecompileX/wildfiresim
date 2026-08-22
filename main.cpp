#include <raylib.h>
#include <ctime>
#include "cellGrid.hpp"


int sWidth = 1080, sHeight = 1080;
const int sFrameRste = 60;

int main(){

    // Load the map file before creating the cell grid
    map mapData;
    mapData.load("test.map");
    cellGrid cell(mapData);


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