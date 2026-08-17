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
    
    cellState cellGridState[16][16][4];

    public:
    
    cellGrid();
    void update(), draw(int sHeight, int sWidth);
};
