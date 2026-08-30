#define RAYGUI_IMPLEMENTATION
#include "cellGrid.hpp"

ui uiO;
int sWidth = 1080, sHeight = 1080;
const int sFrameRste = 240;
bool mapLoaded = false, mapAdded = false;

int main(){

    map mapData;
    std::string mapName;

    std::vector<std::string> mapList;
    mapData.listFiles(mapList);

    InitWindow(sWidth, sHeight, "Wildfire Sim");
    SetTargetFPS(sFrameRste);

    while(mapLoaded == false){
        BeginDrawing();
        ClearBackground(RED);
    
        DrawText("Wildfire sim v0.1", sWidth/2 - .5 * (MeasureText("Wildfire sim  v0.1", 60)), .25 * sHeight, 60, WHITE);
        DrawText("Select map", sWidth/2 - .5 * (MeasureText("Select map", 40)), .4 * sHeight, 40, WHITE);
        for(int x = 0; x < mapList.size(); x++){
            if(GuiButton((Rectangle){25 + ((sWidth - 50) / mapList.size()) * x, sHeight / 2, (sWidth - 50) / mapList.size(), .1 * sHeight}, mapList[x].c_str())){
                mapName = mapList[x];
                mapData.load("maps/" + mapName);
                mapLoaded = true;
                break;
            }
        }
       
        EndDrawing();
    }
    
    cellGrid cell(mapData);
    
    while(!WindowShouldClose()){
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Drawing
        cell.draw(sHeight, sWidth);
        uiO.drawTopBanner();
        uiO.drawButtons(cell.getWaterCooldown(), cell.getRetardantCooldown());

        // Updating
        cell.update(uiO.getLiquidType());

        EndDrawing();

    }
    CloseWindow();
    return 0;
}
