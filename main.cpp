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

        if(GuiButton(Rectangle{sWidth / 2.0f - 100.0f, sHeight * 0.75f, 200.0f, 60.0f}, "Settings")){

        }
       
        EndDrawing();
    }

    std::vector<std::string> musicList = {
        "music/blue-forest.ogg",
        "music/castle-unknown.ogg",
        "music/golden-turrets.ogg",
        "music/night-chip.ogg",
        "music/the-old-master.ogg",
        "music/turtle-nap.ogg"
    };

    InitAudioDevice();
    int musicNumber = GetRandomValue(0, musicList.size() - 1);
    Music music = LoadMusicStream(musicList[musicNumber].c_str());
    PlayMusicStream(music);
    
    cellGrid cell(mapData);
    
    while(!WindowShouldClose()){

        UpdateMusicStream(music);

        if(!IsMusicStreamPlaying(music)){
            UnloadMusicStream(music);

            int newMusicNumber = GetRandomValue(0, musicList.size() - 1);
            while(newMusicNumber == musicNumber){
                newMusicNumber = GetRandomValue(0, musicList.size() - 1);
            }

            musicNumber = newMusicNumber;
            music = LoadMusicStream(musicList[musicNumber].c_str());
            PlayMusicStream(music);
        }
        
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
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
