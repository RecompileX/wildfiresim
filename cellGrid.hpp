#pragma once
#include <raylib.h>
#include <random>
#include <ctime>

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
    int time1;
    
    cellState cellGridState[16][16][4];
    int cellGridTime[16][16][4];

    public:
    
    cellGrid();
    void update(int time2), draw(int sHeight, int sWidth);
};
