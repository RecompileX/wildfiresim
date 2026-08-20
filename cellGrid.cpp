#include "cellGrid.hpp"

cellGrid::cellGrid(){
    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            for (int z = 0; z < 4; z++)
            {
                cellGridState[x][y][z] = treeHealthy;
            }
        }
    }
    srand(time(0));
    int xC = rand()%16, yC = rand()%16, zC = rand()%4;
    cellGridState[xC][yC][zC] = treeBurning;
    cellGridTime[xC][yC][zC] = time(0);

}

void cellGrid::update(int time2){
    time2 = time(0) - 3;
    if(time1 < time2){
        time1 = time2;
        for (int x = 0; x < 16; x++){
            for (int y = 0; y < 16; y++){
                for (int z = 0; z < 4; z++){
                    if(cellGridTime[x][y][z] <= time1 && cellGridState[x][y][z] == treeBurning){

                        cellGridState[x][y][z] = treeBurnt;

                    }
                    else if(cellGridTime[x][y][z] <= (time1 - 3) && cellGridState[x][y][z] == houseBurning){

                        cellGridState[x][y][z] = houseBurnt;

                    }                    
                    
                    if(cellGridState[x][y][z] == treeBurning || cellGridState[x][y][z]  == houseBurning){

                        int nextX = x;
                        int nextY = y;
                        int nextZ = rand() % 4;                  
                    
                        for(int attempt = 0; attempt < 5; attempt++){

                            nextX=x;
                            nextY=y;
                    
                            nextX += (rand() % 3) - 1;
                            nextY += (rand() % 3) - 1;

                            if(nextX >= 0 && nextX < 16 && nextY >= 0 && nextY < 16){

                                if(cellGridState[nextX][nextY][nextZ] == treeHealthy){

                                    cellGridState[nextX][nextY][nextZ] = treeBurning;
                                    cellGridTime[nextX][nextY][nextZ] = time(0);
                                    break;
                                }
                                else if(cellGridState[nextX][nextY][nextZ] == house){

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
}


void cellGrid::draw(int sHeight, int sWidth){

    int cellWidth = sWidth / 16;
    int cellHeight = sHeight / 16;

    int subCellWidth = cellWidth / 2;
    int subCellHeight = cellHeight / 2;
    
    for (int x = 0; x < 16; x++){
    
        for (int y = 0; y < 16; y++){
        
            for (int z = 0; z < 4; z++){

                int drawX = x * cellWidth + (z % 2) * subCellWidth;
                int drawY = y * cellHeight + (z / 2) * subCellHeight;

                switch (cellGridState[x][y][z])
                {
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