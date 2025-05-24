#ifndef GRID_H
#define GRID_H

#include <QString>
#include <vector>

#include "gamestatus.h"

class Grid
{
public:
    Grid();

    enum CellType {
        EMPTY = 0,
        WALL = 1,
        PLAYER = 2,
        TARGET = 3
    };

    GameStatus load(QString filePath);
    const std::vector<std::vector<int>> & getGrid() const;
    int getSize() const;

    bool isPosAvailable(int x, int y);
    void move(int x, int y, int newX, int newY);
    void setCellTypeAtPos(int x, int y, CellType item);

private:
    std::vector<std::vector<int>> _grid;
};

#endif // GRID_H
