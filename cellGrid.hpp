#pragma once
#include <raylib.h>
#include <ctime>
#include "toolbox\toolbox.hpp"
#include "map.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    int timeUpdate, timeWind, mapWidth, mapHeight, subCell, windDirectionV = 0, windSpeed = 0, windSpeedTime = 0;;
    bool firstRun = true;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    std::vector<std::vector<std::vector<int>>> cellGridTime;
    void windDirectionCalculator(int windDirection[2]);

    public:
    
    cellGrid(map& map);
    void update() ;
    void draw(int sHeight, int sWidth);
};
