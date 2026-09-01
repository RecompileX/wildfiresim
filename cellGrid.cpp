#include "cellGrid.hpp"

cellGrid::cellGrid(map& map){
    mapHeight = map.height;
    mapWidth = map.width;
    subCell = map.subCell;

    cellGridState.resize(mapWidth);
    cellGridTime.resize(mapWidth);
    cellGridLiquidTime.resize(mapWidth);

    for (int x = 0; x < mapWidth; x++){
        cellGridState[x].resize(mapHeight);
        cellGridTime[x].resize(mapHeight);
        cellGridLiquidTime[x].resize(mapHeight);
        for (int y = 0; y < mapHeight; y++){
            cellGridState[x][y].resize(subCell);
            cellGridTime[x][y].resize(subCell);
            cellGridLiquidTime[x][y].resize(subCell);
            for (int z = 0; z < subCell; z++){
                cellGridState[x][y][z] = map.mapData[x][y][z];
                if(cellGridState[x][y][z] == treeHealthy || cellGridState[x][y][z] == house){
                    startingStructureCount++;
                }
                if(cellGridState[x][y][z] == house || cellGridState[x][y][z] == water){
                    environmentalTargetCount++;
                }
            }
        }
    }
    timeWind = time(0);
    timeUpdate = time(0);
    waterCooldown = startingWaterCooldown;
    waterCooldownEnd = time(0) + startingWaterCooldown;
    retardantCooldown = startingRetardantCooldown;
    retardantCooldownEnd = time(0) + startingRetardantCooldown;
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

// variables labled time 2 is current time with a delay depending on the function to be comapred with current time

void cellGrid::windDirectionCalculator(int windDirection[2]){
    int time2;
    time2 = time(0) - 15;
    if(firstRun){
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

void cellGrid::applyLiquidToLine(liquidType liquid){
    // Cell sizes
    int cellWidth = GetScreenWidth() / mapWidth;
    int cellHeight = GetScreenHeight() / mapHeight;
    int subCellWidth = cellWidth / 2;
    int subCellHeight = cellHeight / 2;

    // Line distance
    int differenceX = mouseX2 - mouseX1;
    int differenceY = mouseY2 - mouseY1;

    // Pixel steps
    int steps = std::max(std::abs(differenceX), std::abs(differenceY));

    // Prevent divide by zero
    if(steps == 0){
        steps = 1;
    }

    for(int step = 0; step <= steps; step++){
        // Current point
        int screenX = mouseX1 + differenceX * step / steps;
        int screenY = mouseY1 + differenceY * step / steps;

        // Main cell
        int x = screenX / cellWidth;
        int y = screenY / cellHeight;

        // Map bounds
        if(x < 0 || x >= mapWidth || y < 0 || y >= mapHeight){
            continue;
        }

        // Subcell
        int subX = (screenX % cellWidth) / subCellWidth;
        int subY = (screenY % cellHeight) / subCellHeight;
        int z = subY * 2 + subX;

        // Subcell bounds
        if(z < 0 || z >= subCell){
            continue;
        }

        cellState& state = cellGridState[x][y][z];
        cellState previousState = state;

        // Apply water
        if(liquid == waterT){
            if(state == treeBurning){
                state = treePutOut;
            }
            else if(state == houseBurning){
                state = house;
            }
        }
        // Apply retardant
        else if(liquid == retardantT){
            if(state == treeHealthy || state == treeBurning || state == treePutOut){
                state = treeRetardant;
            }
            else if(state == house || state == houseBurning){
                state = houseRetardant;
                harmfulRetardantHits++;
            }
            else if(state == water){
                harmfulRetardantHits++;
            }
        }

        // Liquid time
        if(state != previousState){
            cellGridLiquidTime[x][y][z] = time(0);
        }
    }
}

void cellGrid::update(liquidType liquid){
    if(firstRun){
    windDirectionCalculator(windDirection);
    }
    int currentTime = time(0);

    // Water cooldown
    if(waterCooldownEnd > currentTime){
        waterCooldown = waterCooldownEnd - currentTime;
    }
    else{
        waterCooldown = 0;
    }

    // Retardant cooldown
    if(retardantCooldownEnd > currentTime){
        retardantCooldown = retardantCooldownEnd - currentTime;
    }
    else{
        retardantCooldown = 0;
    }

    int time2 = time(0) - windSpeedTime;
    int timeLine2 = time(0) - 2;

    // Ignore UI clicks
    bool mouseOnMap = GetMouseY() >= GetScreenHeight() / 6;

    // Liquid ready
    bool liquidReady =
        (liquid == waterT && waterCooldown == 0) ||
        (liquid == retardantT && retardantCooldown == 0);

    if(!waitingForClick2 && !lineExists){
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseOnMap && liquidReady){

            // Line start
            mouseX1 = GetMouseX();
            mouseY1 = GetMouseY();
            waitingForClick2 = true;

        }
    }
    else if(waitingForClick2){
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseOnMap){
            if(liquidReady){
                // Line end
                mouseX2 = GetMouseX();
                mouseY2 = GetMouseY(); 
                float dy = mouseY2 - mouseY1;
                float dx = mouseX2 - mouseX1;
                length = std::sqrt(dx * dx + dy * dy);
                angle = std::atan2f(dy, dx) * RAD2DEG;
                float dxCells = dx / (GetScreenWidth() / mapWidth);
                float dyCells = dy / (GetScreenHeight() / mapHeight);
                float lengthCells = std::sqrt(dxCells * dxCells + dyCells * dyCells);

                if((liquid == waterT && lengthCells <= maxWaterLength) ||
                   (liquid == retardantT && lengthCells <= maxRetardantLength)){

                    timeLine = time(0);
                    lineLiquid = liquid;
                    lineExists = true;
                    waitingForClick2 = false;
                }
                else if(liquid == waterT){
                    waterLengthExceeded = true;
                    retardantLengthExceeded = false;
                    timeText = time(0);
                    waitingForClick2 = false;
                }
                else if(liquid == retardantT){
                    retardantLengthExceeded = true;
                    waterLengthExceeded = false;
                    timeText = time(0);
                    waitingForClick2 = false;
                }
            }
        }
    }

    // Drop liquid
    if(lineExists && timeLine <= timeLine2){
        applyLiquidToLine(lineLiquid);
        lineExists = false;

        // Start cooldown
        if(lineLiquid == waterT){
            waterCooldown = waterCooldownDuration;
            waterCooldownEnd = currentTime + waterCooldownDuration;
        }
        else if(lineLiquid == retardantT){
            retardantCooldown = retardantCooldownDuration;
            retardantCooldownEnd = currentTime + retardantCooldownDuration;
        }
    }
    
    if(timeUpdate <= time2){
        timeUpdate = time(0);
        windDirectionCalculator(windDirection);
        // Previous update
        auto oldGridState = cellGridState;
        bool fireExists = false;
        int survivingStructures = 0;
        
        // Map cells
        for (int x = 0; x < mapWidth; x++){
            for (int y = 0; y < mapHeight; y++){
                // Subcells
                for (int z = 0; z < subCell; z++){
                    // Victory Check
                    if(cellGridState[x][y][z] == treeBurning || cellGridState[x][y][z] == houseBurning){
                        fireExists = true;
                    }
                    if(cellGridState[x][y][z] == treeHealthy || cellGridState[x][y][z] == treePutOut ||
                       cellGridState[x][y][z] == treeRetardant || cellGridState[x][y][z] == house ||
                       cellGridState[x][y][z] == houseRetardant){
                        survivingStructures++;
                    }
                    // Burnout / Evap
                    if(cellGridTime[x][y][z] <= timeUpdate - treeBurnTime && oldGridState[x][y][z] == treeBurning){

                        cellGridState[x][y][z] = treeBurnt;

                    }
                    else if(cellGridTime[x][y][z] <= timeUpdate - houseBurnTime && oldGridState[x][y][z] == houseBurning){

                        cellGridState[x][y][z] = houseBurnt;

                    }                    
                    if(cellGridLiquidTime[x][y][z] <= timeUpdate - waterEvapTime && cellGridState[x][y][z] == treePutOut){

                        cellGridState[x][y][z] = treeHealthy;

                    }
                    else if(cellGridLiquidTime[x][y][z] <= timeUpdate - retardantEvapTime && cellGridState[x][y][z] == treeRetardant){

                        cellGridState[x][y][z] = treeHealthy;

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
                                bool spreadingWithWind = false;

                                bool spreadingAgainstWind = false;

                                if(dx == windDirection[0] && dy == windDirection[1]){
                                
                                    spreadingWithWind = true;
                                }
                                if(dx == -windDirection[0] && dy == -windDirection[1]){
                                
                                    spreadingAgainstWind = true;
                                }
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
        victory = !fireExists;
        if(victory && startingStructureCount > 0){
            float survivalPercent = (float)survivingStructures / startingStructureCount;
            float cleanPercent = 1.0f;
            if(environmentalTargetCount > 0){
                cleanPercent -= (float)harmfulRetardantHits / environmentalTargetCount;
            }
            cleanPercent = std::clamp(cleanPercent, 0.0f, 1.0f);
            score = std::clamp((int)std::round(survivalPercent * 750 + cleanPercent * 250), 0, 1000);
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
                    case water:
                        DrawRectangle(drawX, drawY, subCellWidth, subCellHeight, BLUE);
                        break;
                }
            }
        }
    }
    if(lineExists){

        // Line width
        float lineThickness = subCellWidth / 2;

        // Line color
        Color lineColor = BLUE;
        if(lineLiquid == retardantT){
            lineColor = PURPLE;
        }

        // Draw line
        DrawRectanglePro(
            Rectangle{(float)mouseX1, (float)mouseY1, length, lineThickness},
            Vector2{0, lineThickness / 2.0f},
            angle,
            lineColor
        );
    }
    if(waterLengthExceeded && timeText >= time(0) - 3){
        DrawText("Water length exceeded! Max length is 12 cells!", GetScreenWidth()/2 - MeasureText("Water length exceeded! Max length is 12 cells!", 40)/2, GetScreenHeight()/2, 40, RED);
    }
    else{
        waterLengthExceeded = false;
    }
    if(retardantLengthExceeded && timeText >= time(0) - 3){
        DrawText("Retardant length exceeded! Max length is 6 cells!", GetScreenWidth()/2 - MeasureText("Retardant length exceeded! Max length is 6 cells!", 40)/2, GetScreenHeight()/2, 40, RED);
    }
    else{
        retardantLengthExceeded = false;
    }
}
