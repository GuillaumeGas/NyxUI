#include "gameinfo.h"

GameInfo::GameInfo() : _botPos(0, 0)
{

}

Grid::Status GameInfo::loadGrid(QString filePath)
{
    auto status = _grid.load(filePath);;
    if (status == Grid::Status::OK)
        _botPos = Position(0, 0);
    return status;
}

const Grid * GameInfo::getGrid() const
{
    return &_grid;
}

void GameInfo::tryMoveBot(const Position & newPos)
{
    if (_grid.isPosAvailable(newPos.x, newPos.y))
    {
        _grid.move(_botPos.x, _botPos.y, newPos.x, newPos.y);
        _botPos = newPos;
    }
}

void GameInfo::moveRight()
{
    Position newPos = _botPos;
    newPos.x++;

    tryMoveBot(newPos);
}

void GameInfo::moveLeft()
{
    Position newPos = _botPos;
    newPos.x--;

    tryMoveBot(newPos);
}

void GameInfo::moveUp()
{
    Position newPos = _botPos;
    newPos.y--;

    tryMoveBot(newPos);
}

void GameInfo::moveDown()
{
    Position newPos = _botPos;
    newPos.y++;

    tryMoveBot(newPos);
}

const Position & GameInfo::getBot() const
{
    return _botPos;
}
