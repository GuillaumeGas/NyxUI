#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QString>

#include "grid.h"

class Position
{
public:
    Position(int x, int y) : x(x), y(y) {}

    int x;
    int y;
};

class GameInfo
{
public:
    GameInfo();

    Grid::Status loadGrid(QString filePath);

    const Grid * getGrid() const;

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    // Will update grid AND bot position
    void tryMoveBot(const Position & p2);

    const Position & getBot() const;

private:
    Grid _grid;
    Position _botPos;
};

#endif // GAMEINFO_H
