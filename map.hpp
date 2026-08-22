#pragma once
#include <fstream>
#include <string>

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

struct map{
    
    cellState mapData[16][16][4];

    void load(const std::string& filename){
        std::ifstream file(filename);

        if (!file.is_open())
            return;

        std::string cell;

        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < 16; y++)
            {
                file >> cell;

                for (int z = 0; z < 4; z++)
                {
                    switch (cell[z])
                    {
                        case 'T':
                            mapData[x][y][z] = treeHealthy;
                            break;

                        case 'W':
                            mapData[x][y][z] = water;
                            break;

                        case 'R':
                            mapData[x][y][z] = road;
                            break;

                        case 'H':
                            mapData[x][y][z] = house;
                            break;
                    }
                }
            }
        }
    }
};