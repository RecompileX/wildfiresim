#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace lineCalculation {

struct selectedSubCell {
    int x;
    int y;
    int z;
};

struct point {
    float x;
    float y;
};

inline float projection(const point& corner, const point& axis)
{
    return corner.x * axis.x + corner.y * axis.y;
}

inline bool lineOverlapsCell(
    const std::array<point, 4>& lineCorners,
    float cellLeft,
    float cellTop,
    float cellRight,
    float cellBottom,
    const point& lineDirection,
    const point& lineSide)
{
    const std::array<point, 4> cellCorners{{
        {cellLeft, cellTop},
        {cellRight, cellTop},
        {cellRight, cellBottom},
        {cellLeft, cellBottom}
    }};
    const std::array<point, 4> axes{{
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        lineDirection,
        lineSide
    }};

    for (const point& axis : axes) {
        float lineMinimum = projection(lineCorners[0], axis);
        float lineMaximum = lineMinimum;
        float cellMinimum = projection(cellCorners[0], axis);
        float cellMaximum = cellMinimum;

        for (int corner = 1; corner < 4; corner++) {
            const float lineValue = projection(lineCorners[corner], axis);
            const float cellValue = projection(cellCorners[corner], axis);

            lineMinimum = std::min(lineMinimum, lineValue);
            lineMaximum = std::max(lineMaximum, lineValue);
            cellMinimum = std::min(cellMinimum, cellValue);
            cellMaximum = std::max(cellMaximum, cellValue);
        }

        if (lineMaximum < cellMinimum || cellMaximum < lineMinimum) {
            return false;
        }
    }

    return true;
}

// Returns every subcell touched by the full width of the drawn line.
inline std::vector<selectedSubCell> calculateSubCells(
    int mouseX1,
    int mouseY1,
    int mouseX2,
    int mouseY2,
    float lineThickness,
    int cellWidth,
    int cellHeight,
    int mapWidth,
    int mapHeight,
    int subCellCount)
{
    std::vector<selectedSubCell> cells;

    if (cellWidth <= 0 || cellHeight <= 0 || lineThickness <= 0.0f || subCellCount <= 0) {
        return cells;
    }

    const float differenceX = static_cast<float>(mouseX2 - mouseX1);
    const float differenceY = static_cast<float>(mouseY2 - mouseY1);
    const float lineLength = std::sqrt(
        differenceX * differenceX + differenceY * differenceY
    );

    if (lineLength == 0.0f) {
        const int x = mouseX1 / cellWidth;
        const int y = mouseY1 / cellHeight;
        const int columns = subCellCount / 2;
        const int subCellWidth = cellWidth / columns;
        const int subCellHeight = cellHeight / 2;
        const int localX = (mouseX1 - x * cellWidth) / subCellWidth;
        const int localY = (mouseY1 - y * cellHeight) / subCellHeight;
        const int z = localY * columns + localX;

        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight && z >= 0 && z < subCellCount) {
            cells.push_back({x, y, z});
        }

        return cells;
    }

    const point lineDirection{
        differenceX / lineLength,
        differenceY / lineLength
    };
    const point lineSide{-lineDirection.y, lineDirection.x};
    const float halfThickness = lineThickness / 2.0f;
    const point sideOffset{
        lineSide.x * halfThickness,
        lineSide.y * halfThickness
    };
    const std::array<point, 4> lineCorners{{
        {mouseX1 + sideOffset.x, mouseY1 + sideOffset.y},
        {mouseX2 + sideOffset.x, mouseY2 + sideOffset.y},
        {mouseX2 - sideOffset.x, mouseY2 - sideOffset.y},
        {mouseX1 - sideOffset.x, mouseY1 - sideOffset.y}
    }};

    const int columns = subCellCount / 2;
    const int subCellWidth = cellWidth / columns;
    const int subCellHeight = cellHeight / 2;

    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            for (int z = 0; z < subCellCount; z++) {
                const float cellLeft = static_cast<float>(
                    x * cellWidth + (z % columns) * subCellWidth
                );
                const float cellTop = static_cast<float>(
                    y * cellHeight + (z / columns) * subCellHeight
                );
                const float cellRight = cellLeft + subCellWidth;
                const float cellBottom = cellTop + subCellHeight;

                if (lineOverlapsCell(
                        lineCorners,
                        cellLeft,
                        cellTop,
                        cellRight,
                        cellBottom,
                        lineDirection,
                        lineSide)) {
                    cells.push_back({x, y, z});
                }
            }
        }
    }

    return cells;
}

}
