#pragma once
#include <raylib.h>
#include <ctime>
#include <algorithm>
#include <cmath>
#include "toolbox\toolbox.hpp"
#include "map.hpp"
#include "ui.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    const int treeBurnTime = 8;
    const int houseBurnTime = 15;
    const int startingWaterCooldown = 10;
    const int startingRetardantCooldown = 15;
    const int waterCooldownDuration = 3;
    const int retardantCooldownDuration = 6;
    const int retardantEvapTime = 30;
    const int waterEvapTime = 6;
    const int maxRetardantLength = 6;
    const int maxWaterLength = 12;
    

    int mouseX1, mouseX2, mouseY1, mouseY2,
    timeUpdate, timeWind, timeLine,
    mapWidth, mapHeight, subCell, cellSize, mapStartX, mapStartY,
    windDirectionV = 0, windSpeed = 0, windSpeedTime = 0, 
    waterCooldown = 0, waterCooldownEnd = 0,
    retardantCooldown = 0, retardantCooldownEnd = 0,
    timeText = 0, startingStructureCount = 0, environmentalTargetCount = 0,
    harmfulRetardantHits = 0, score = 0;

    float length, angle;
    bool firstRun = true, waitingForClick2 = false, lineExists = false, waterLengthExceeded = false, retardantLengthExceeded = false;
    liquidType lineLiquid = waterT;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    std::vector<std::vector<std::vector<int>>> cellGridTime;
    std::vector<std::vector<std::vector<int>>> cellGridLiquidTime;
    void windDirectionCalculator(int windDirection[2]);
    void applyLiquidToLine(liquidType liquid);

    public:

    cellGrid(map& map);
    bool victory = false;
    void update(liquidType liquid);
    void draw(int sHeight, int sWidth);
    int getWaterCooldown() const { return waterCooldown; }
    int getRetardantCooldown() const { return retardantCooldown; }
    int getScore() const { return score; }
};
