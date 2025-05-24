#ifndef GAMEINFO_H
#define GAMEINFO_H

#include <QString>
#include <QTextStream>

#include "grid.h"
#include "gamestatus.h"

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

    GameStatus loadGame(QString filePath);

    const Grid * getGrid() const;

    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();

    // Will update grid AND bot position
    void tryMoveBot(const Position & p2);

    const Position & getBot() const;

private:
    GameStatus _loadHeader(QString filePath);
    GameStatus _readPositionInHeader(QTextStream & stream, Position & pos);
    GameStatus _readSingleIntInHeader(QTextStream & stream, unsigned int & pos);

    Grid _grid;
    Position _botPos;
    Position _targetPos;
    unsigned int _turnsLimit;
};

#endif // GAMEINFO_H
