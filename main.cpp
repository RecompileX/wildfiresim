#include <raylib.h>
#include <ctime>

int sWidth = 1280, sHeight = 800;
const int sFrameRste = 60;

int main(){

    InitWindow(sWidth, sHeight, "Wildfire Sim");
    SetTargetFPS(sFrameRste);

    while(!WindowShouldClose()){
        
        ClearBackground(BLACK);
        BeginDrawing();

        EndDrawing();

    }
    return 0;
}