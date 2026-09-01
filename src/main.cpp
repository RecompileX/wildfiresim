#define RAYGUI_IMPLEMENTATION
#include "cellGrid.hpp"
#include <cstdlib>

ui uiO;
int sWidth = 1080, sHeight = 1080;
const int sFrameRate = 240;
bool mapLoaded = false, inSettings = false, editVolume = false;
char volumeText[4] = "50";
int volume = 50;

int main(){

    map mapData;
    std::string mapName;

    std::vector<std::string> mapList;
    mapData.listFiles(mapList);

    InitWindow(sWidth, sHeight, "WildfireSim");
    SetTargetFPS(sFrameRate);

    while(mapLoaded == false && !WindowShouldClose()){
        BeginDrawing();

        if(inSettings == false){
            ClearBackground(RED);
        
            DrawText("Wildfire Sim v0.3", sWidth/2 - .5 * (MeasureText("Wildfire sim  v0.1", 60)), .25 * sHeight, 60, WHITE);
            DrawText("Select map", sWidth/2 - .5 * (MeasureText("Select map", 40)), .4 * sHeight, 40, WHITE);
            for(int x = 0; x < mapList.size(); x++){
                if(GuiButton((Rectangle){25 + ((sWidth - 50) / mapList.size()) * x, sHeight / 2, (sWidth - 50) / mapList.size(), .1 * sHeight}, mapList[x].c_str())){
                    mapName = mapList[x];
                    mapData.load("assets/maps/" + mapName);
                    mapLoaded = true;
                    break;
                }
            }
            if(GuiButton(Rectangle{sWidth / 2.0f - 100.0f, sHeight * 0.75f, 200.0f, 60.0f}, "Settings")){
                inSettings = true;
            }
        }
        else{
            ClearBackground(Color{35, 45, 60, 255});

            DrawText("Volume:", 100, 100, 40, WHITE);

            if(GuiTextBox(Rectangle{280, 90, 220, 60}, volumeText, 4, editVolume)){
                editVolume = !editVolume;

                if(!editVolume){
                    volume = std::clamp(std::atoi(volumeText), 0, 100);
                }
            }

            if(GuiButton(Rectangle{sWidth / 2.0f - 100.0f, sHeight * 0.75f, 200.0f, 60.0f}, "Exit Settings")){
                
                inSettings = false;
            }
        }

        EndDrawing();
    }

    if(!mapLoaded){
        CloseWindow();
        return 0;
    }

    std::vector<std::string> musicList = {
        "assets/music/blue-forest.ogg",
        "assets/music/castle-unknown.ogg",
        "assets/music/golden-turrets.ogg",
        "assets/music/night-chip.ogg",
        "assets/music/the-old-master.ogg",
        "assets/music/turtle-nap.ogg"
    };

    InitAudioDevice();
    SetMasterVolume(volume / 100.0f);
    int musicNumber = GetRandomValue(0, musicList.size() - 1);
    Music victoryMusic = LoadMusicStream("assets/music/victory.ogg");
    Music music = LoadMusicStream(musicList[musicNumber].c_str());
    PlayMusicStream(music);
    
    cellGrid cell(mapData);
    
    while(!WindowShouldClose()){

        if(cell.victory){
            UpdateMusicStream(victoryMusic);
        }
        else{
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
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Drawing
        cell.draw(sHeight, sWidth);
        uiO.drawTopBanner();
        uiO.drawButtons(cell.getWaterCooldown(), cell.getRetardantCooldown());

        // Updating
        cell.update(uiO.getLiquidType());

        if(cell.victory && IsMusicStreamPlaying(music)){
            StopMusicStream(music);
            PlayMusicStream(victoryMusic);
        }

        if(cell.victory){
            const char* victoryText = "You Put Out The Fire!";
            DrawText(victoryText, GetScreenWidth()/2 - MeasureText(victoryText, 40)/2, GetScreenHeight()/2, 40, WHITE);

            const char* scoreText = TextFormat("Score: %d / 1000", cell.getScore());
            DrawText(scoreText, GetScreenWidth()/2 - MeasureText(scoreText, 30)/2, GetScreenHeight()/2 + 50, 30, WHITE);
        }

        EndDrawing();
    }

    UnloadMusicStream(music);
    UnloadMusicStream(victoryMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
