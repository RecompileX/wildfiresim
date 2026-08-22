#pragma once
#include <raylib.h>
#include <random>
#include <ctime>
#include "toolbox\toolbox.hpp"

enum cellState {
    treeHealthy,
    treeBurnt,
    treeBurning,
    treePutOut,
    treeRetardant,

    water,
    road,
    house,
    houseBurnt,
    houseBurning,
    houseRetardant
};


class cellGrid
{
    private:
    int cellXcellY[16][16][2];
    int windDirection[2];
    int timeUpdate, timeWind, windDirectionV = 0, windSpeed = 0;
    bool firstRun = true;

    cellState cellGridState[16][16][4];
    int cellGridTime[16][16][4];
    void windDirectionCalculator(int windDirectionSpeed[2]);

    public:
    
    cellGrid();
    void update(int time2), draw(int sHeight, int sWidth);
};
