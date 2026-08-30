#pragma once
#include <raylib.h>
#include <ctime>
#include <cmath>
#include "toolbox\toolbox.hpp"
#include "map.hpp"
#include "ui.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    static constexpr int treeBurnTime = 8;
    static constexpr int houseBurnTime = 15;

    int mouseX1, mouseX2, mouseY1, mouseY2,
    timeUpdate, timeWind, 
    mapWidth, mapHeight, subCell, 
    windDirectionV = 0, windSpeed = 0, windSpeedTime = 0;
    float length, angle;
    bool firstRun = true, watingForClick2 = false, lineExists = false;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    std::vector<std::vector<std::vector<int>>> cellGridTime;
    void windDirectionCalculator(int windDirection[2]);

    public:
    
    cellGrid(map& map);
    void update();
    void draw(int sHeight, int sWidth);
};
