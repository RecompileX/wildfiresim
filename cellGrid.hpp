#pragma once
#include <raylib.h>
#include <ctime>
#include <cmath>
#include "toolbox\toolbox.hpp"
#include "map.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    static constexpr int treeBurnTime = 8;
    static constexpr int houseBurnTime = 15;

    int timeUpdate, timeWind, mapWidth, mapHeight, subCell, windDirectionV = 0, windSpeed = 0, windSpeedTime = 0;
    bool firstRun = true;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    std::vector<std::vector<std::vector<int>>> cellGridTime;
    void windDirectionCalculator(int windDirection[2]);

    public:
    
    cellGrid(map& map);
    void update(int mouseX, int mouseY);
    void draw(int sHeight, int sWidth);
};
