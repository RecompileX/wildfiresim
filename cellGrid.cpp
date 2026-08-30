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
        int xC = toolbox::random(0, mapWidth - 1), yC = toolbox::random(0, mapHeight - 1), zC = toolbox::random(0, subCell - 1);
        if(cellGridState[xC][yC][zC] == treeHealthy){
            cellGridState[xC][yC][zC] = treeBurning;
            cellGridTime[xC][yC][zC] = time(0);
            fireStarted = true;
        }
    }
}

// variables labled time 2 is current time with a delay depending on the function to be comapred with current time at delay

void cellGrid::windDirectionCalculator(int windDirection[2]){
    int time2;
    time2 = time(0) - 15;
    if(firstRun == true){
        windSpeed = toolbox::random(0, 90);
        windDirectionV = toolbox::random(0, 7);
        firstRun = false;
    }
    if(timeWind <= time2){
        timeWind = time(0);
        windSpeed = toolbox::random(0, 90);
        windDirectionV = toolbox::random(0, 7);
    }
    if(!toolbox::chance(windSpeed)){

        // 0 is for x, -1 meaning left 1 meaning right 1 is for y with 1 being up and -1 being down
        windDirection[0] = toolbox::random(-1, 1);
        windDirection[1] = toolbox::random(-1, 1);
            
    }
    else{
        switch(windDirectionV){
            case 0:
                windDirection[0] = -1;
                windDirection[1] = 1;
                break;
            case 1:
                windDirection[0] = 0;
                windDirection[1] = 1;
                break;
            case 2:
                windDirection[0] = 1;
                windDirection[1] = 1;
                break;
            case 3:
                windDirection[0] = 1;
                windDirection[1] = 0;
                break;
            case 4:
                windDirection[0] = 1;
                windDirection[1] = -1;
                break;
            case 5:
                windDirection[0] = 0;
                windDirection[1] = -1;
                break;
            case 6:
                windDirection[0] = -1;
                windDirection[1] = -1;
                break;
            case 7:
                windDirection[0] = -1;
                windDirection[1] = 0;
                break;
        }
    }
    windSpeedTime = 1 + round(2.0 * log(91.0 - windSpeed) / log(91.0));
}

void cellGrid::update(int mouseX, int mouseY){
    if(firstRun == true){
    windDirectionCalculator(windDirection);
    }
    int time2 = time(0) - windSpeedTime;
    if(timeUpdate <= time2){
        timeUpdate = time(0);
        windDirectionCalculator(windDirection);
        // Previous update
        auto oldGridState = cellGridState;
        
        // Map cells
        for (int x = 0; x < mapWidth; x++){
            for (int y = 0; y < mapHeight; y++){
                // Subcells
                for (int z = 0; z < subCell; z++){
                    // Burnout
                    if(cellGridTime[x][y][z] <= timeUpdate - treeBurnTime && oldGridState[x][y][z] == treeBurning){

                        cellGridState[x][y][z] = treeBurnt;

                    }
                    else if(cellGridTime[x][y][z] <= timeUpdate - houseBurnTime && oldGridState[x][y][z] == houseBurning){

                        cellGridState[x][y][z] = houseBurnt;

                    }                    
                    // Spread fire
                    if(oldGridState[x][y][z] == treeBurning || oldGridState[x][y][z] == houseBurning){
                        // Eight neighbors
                        for(int dx = -1; dx <= 1; dx++){
                            for(int dy = -1; dy <= 1; dy++){
                                // Skip center
                                if(dx == 0 && dy == 0){
                                    continue;
                                }

                                int nextX = x + dx;
                                int nextY = y + dy;

                                // Map bounds
                                if(nextX < 0 || nextX >= mapWidth || nextY < 0 || nextY >= mapHeight){
                                    continue;
                                }

                                // Wind bias
                                bool spreadingWithWind =
                                    dx == windDirection[0] && dy == windDirection[1];

                                bool spreadingAgainstWind =
                                    dx == -windDirection[0] && dy == -windDirection[1];

                                int spreadChance = 10; // Sideways

                                if(spreadingWithWind){
                                    spreadChance = 80; // Downwind
                                }
                                else if(spreadingAgainstWind){
                                    spreadChance = 5; // Upwind
                                }

                                // Failed ignition
                                if(!toolbox::chance(spreadChance)){
                                    continue;
                                }

                                // Target subcell
                                int nextZ = toolbox::random(0, subCell - 1);

                                // Ignite fuel
                                if(oldGridState[nextX][nextY][nextZ] == treeHealthy){
                                    cellGridState[nextX][nextY][nextZ] = treeBurning;
                                    cellGridTime[nextX][nextY][nextZ] = time(0);
                                }
                                else if(oldGridState[nextX][nextY][nextZ] == house){
                                    cellGridState[nextX][nextY][nextZ] = houseBurning;
                                    cellGridTime[nextX][nextY][nextZ] = time(0);
                                }
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
