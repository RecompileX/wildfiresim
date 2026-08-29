#include "cellGrid.hpp"

cellGrid::cellGrid(map& map){
    mapHeight = map.height;
    mapWidth = map.width;
    subCell = map.subCell;

    cellGridState.resize(mapWidth);
    cellGridTime.resize(mapWidth);

    for (int x = 0; x < mapWidth; x++){
        cellGridState[x].resize(mapHeight);
        cellGridTime[x].resize(mapHeight);
        for (int y = 0; y < mapHeight; y++){
            cellGridState[x][y].resize(subCell);
            cellGridTime[x][y].resize(subCell);
            for (int z = 0; z < subCell; z++){
                cellGridState[x][y][z] = map.mapData[x][y][z];
            }
        }
    }
    timeWind = time(0);
    timeUpdate = time(0);
    srand(time(0));
    bool fireStarted = false;
    while(fireStarted == false){
        int xC = rand()%mapWidth, yC = rand()%mapHeight, zC = rand()%subCell;
        if(cellGridState[xC][yC][zC] == treeHealthy){
            cellGridState[xC][yC][zC] = treeBurning;
            cellGridTime[xC][yC][zC] = time(0);
            fireStarted = true;
        }
    }
}

// variables labled time 2 is current time with a delay depending on the function

void cellGrid::windDirectionCalculator(int windDirectionSpeed[2]){
    int time2;
    time2 = time(0) - 15;
    if(firstRun == true){
        windSpeed = toolbox::random(0, 90);
        windDirectionV = toolbox::random(0, 7);
        firstRun = false;
    }
    if(timeWind <= time2){
        timeWind = time2;
        windSpeed = toolbox::random(0, 90);
        windDirectionV = toolbox::random(0, 7);
    }
    if(!toolbox::chance(windSpeed)){

        // 0 is for x, -1 meaning left 1 meaning right 1 is for y with 1 being up and -1 being down
        windDirectionSpeed[0] = toolbox::random(-1, 1);
        windDirectionSpeed[1] = toolbox::random(-1, 1);
            
    }
    else{
        switch(windDirectionV){
            case 0:
                windDirectionSpeed[0] = -1;
                windDirectionSpeed[1] = 1;
                break;
            case 1:
                windDirectionSpeed[0] = 0;
                windDirectionSpeed[1] = 1;
                break;
            case 2:
                windDirectionSpeed[0] = 1;
                windDirectionSpeed[1] = 1;
                break;
            case 3:
                windDirectionSpeed[0] = 1;
                windDirectionSpeed[1] = 0;
                break;
            case 4:
                windDirectionSpeed[0] = 1;
                windDirectionSpeed[1] = -1;
                break;
            case 5:
                windDirectionSpeed[0] = 0;
                windDirectionSpeed[1] = -1;
                break;
            case 6:
                windDirectionSpeed[0] = -1;
                windDirectionSpeed[1] = -1;
                break;
            case 7:
                windDirectionSpeed[0] = -1;
                windDirectionSpeed[1] = 0;
                break;
        }
    }
}

void cellGrid::update(){
    int time2 = time(0) - 3;
    if(timeUpdate < time2){
        timeUpdate = time2;
        windDirectionCalculator(windDirection);
        auto oldGridState = cellGridState;
        
        for (int x = 0; x < mapWidth; x++){
            for (int y = 0; y < mapHeight; y++){
                for (int z = 0; z < subCell; z++){
                    if(cellGridTime[x][y][z] <= timeUpdate - 3 && oldGridState[x][y][z] == treeBurning){

                        cellGridState[x][y][z] = treeBurnt;

                    }
                    else if(cellGridTime[x][y][z] <= timeUpdate - 6 && oldGridState[x][y][z] == houseBurning){

                        cellGridState[x][y][z] = houseBurnt;

                    }                    
                    
                    if(oldGridState[x][y][z] == treeBurning || oldGridState[x][y][z]  == houseBurning){

                        int nextX = x;
                        int nextY = y;
                        int nextZ = rand() % subCell;                  

                        nextX=x;
                        nextY=y;
                
                        nextX += windDirection[0];
                        nextY += windDirection[1];

                        if(nextX >= 0 && nextX < mapWidth && nextY >= 0 && nextY < mapHeight){

                            if(oldGridState[nextX][nextY][nextZ] == treeHealthy){

                                cellGridState[nextX][nextY][nextZ] = treeBurning;
                                cellGridTime[nextX][nextY][nextZ] = time(0);
                                break;
                            }
                            else if(oldGridState[nextX][nextY][nextZ] == house){

                                cellGridState[nextX][nextY][nextZ] = houseBurning;
                                cellGridTime[nextX][nextY][nextZ] = time(0);
                                break;
                            }
                        }
                    } 
                }
            }
        }
    }
}


void cellGrid::draw(int sHeight, int sWidth){

    int cellWidth = sWidth / mapWidth;
    int cellHeight = sHeight / mapHeight;

    int subCellWidth = cellWidth / 2;
    int subCellHeight = cellHeight / 2;
    
    for (int x = 0; x < mapWidth; x++){
        for (int y = 0; y < mapHeight; y++){
            for (int z = 0; z < subCell; z++){

                int drawX = x * cellWidth + (z % (subCell / 2)) * subCellWidth;
                int drawY = y * cellHeight + (z / (subCell / 2)) * subCellHeight;

                switch (cellGridState[x][y][z]){
                    case treeHealthy:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, GREEN);
                        break;

                    case treeBurning:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, RED);
                        break;

                    case treeBurnt:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, DARKGRAY);
                        break;

                    case treePutOut:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, BLUE);
                        break;

                    case treeRetardant:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, PURPLE);
                        break;

                    case road:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, GRAY);
                        break;

                    case house:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, BROWN);
                        break;

                    case houseBurnt:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, BLACK);
                        break;

                    case houseBurning:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, ORANGE);
                        break;

                    case houseRetardant:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, PURPLE);
                        break;
                }
            }
        }
    }
}