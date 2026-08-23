#pragma once
#include <raylib.h>
#include <random>
#include <ctime>
#include "toolbox\toolbox.hpp"
#include "map.hpp"

class cellGrid
{
    private:
    int windDirection[2];
    int timeUpdate, timeWind, windDirectionV = 0, windSpeed = 0;
    bool firstRun = true;

    std::vector<std::vector<std::vector<cellState>>> cellGridState;
    int cellGridTime[16][16][4];
    void windDirectionCalculator(int windDirectionSpeed[2]);

    public:
    
    cellGrid(map& map);
    void update(int time2), draw(int sHeight, int sWidth);
};
