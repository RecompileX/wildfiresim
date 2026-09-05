#pragma once
#include <fstream>
#include <filesystem>
#include <string>
#include <print>
#include <vector>
#include <sstream>

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

namespace fs = std::filesystem;

struct map {

    std::vector<std::vector<std::vector<cellState>>> mapData;

    int width = 0, height = 0, subCell = 4;

    void listFiles(std::vector<std::string>& mapList)
{
    for (const auto& file : std::filesystem::directory_iterator("assets/maps")){
        if (file.path().extension() == ".map"){
            
            mapList.push_back(file.path().filename().string());
        }
    }
}
    
    bool load(const std::string& filename) {

        std::ifstream file(filename);

        if (!file.is_open()){
            std::println("Map file not found");
            return false;
        }

        // Read the first line
        std::string header;
        std::getline(file, header);

        // Header must be in the format #widthxheight
        if (header.size() < 4 || header[0] != '#'){
            std::println("Header not in correct format or not found");
            return false;
        }
        // find the x ex: 16"x"16
        size_t xPosition = header.find('x');

        if (xPosition == std::string::npos){
            std::println("xPosition returned npos integer");
            return false;
        }
        

        // get width height from header
        std::istringstream dimensions(header.substr(1));
        char separator;
        if (!(dimensions >> width >> separator >> height) || separator != 'x'){
            std::println("Invalid dimensions");
            return false;
        }
        dimensions >> std::ws;
        if (!dimensions.eof()){
            std::println("Invalid dimensions");
            return false;
        }

        // Dimensions check
        if (width <= 0 || height <= 0){
            std::println("Invalid dimensions");
            return false;
        }

        // resize map
        mapData.resize(width);

        for (int x = 0; x < width; x++)
        {
            mapData[x].resize(height);

            for (int y = 0; y < height; y++)
            {
                mapData[x][y].resize(subCell);
            }
        }

        std::string cell;
        bool hasTree = false;

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (!(file >> cell)){
                    std::println("Map is incomplete" );
                    return false;
                }

                // Grid box has only correct numbers of subCell check
                if (cell.size() != subCell)
                    return false;

                for (int z = 0; z < subCell; z++)
                {
                    switch (cell[z])
                    {
                        case 'T':
                            mapData[x][y][z] = treeHealthy;
                            hasTree = true;
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

                        default:
                            std::println("Invalid cell type");
                            return false;
                    }
                }
            }
        }
        if (!hasTree){
            std::println("Map needs at least one tree" );
            return false;
        }
        return true;
    }
};
