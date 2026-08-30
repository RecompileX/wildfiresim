#pragma once
#include <raylib.h>
#include <ctime>
#include "toolbox\toolbox.hpp"
#include "map.hpp"
#include "ui.hpp"
#include "lineCalculation\lineCalculation.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    static constexpr int treeBurnTime = 8;
    static constexpr int houseBurnTime = 15;
    static constexpr int startingWaterCooldown = 10;
    static constexpr int startingRetardantCooldown = 15;
    static constexpr int waterCooldownDuration = 3;
    static constexpr int retardantCooldownDuration = 6;

    int mouseX1, mouseX2, mouseY1, mouseY2,
    timeUpdate, timeWind, 
    mapWidth, mapHeight, subCell, 
    windDirectionV = 0, windSpeed = 0, windSpeedTime = 0, 
    waterCooldown = 0, waterCooldownEnd = 0, waterEvapTime = 6,
    retardantCooldown = 0, retardantCooldownEnd = 0, retardantEvapTime = 30;
    float length, angle;
    bool firstRun = true, waitingForClick2 = false, lineExists = false;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    std::vector<std::vector<std::vector<int>>> cellGridTime;
    std::vector<std::vector<std::vector<int>>> cellGridLiquidTime;
    std::vector<lineCalculation::selectedSubCell> lineCells;
    void windDirectionCalculator(int windDirection[2]);
    void applyLiquidToLine(liquidType liquid);

    public:
    
    cellGrid(map& map);
    void update(liquidType liquid);
    void draw(int sHeight, int sWidth);
    int getWaterCooldown() const { return waterCooldown; }
    int getRetardantCooldown() const { return retardantCooldown; }
};
