#include "gameinfo.h"

GameInfo::GameInfo() : _botPos(0, 0)
{

}

Grid::Status GameInfo::loadGrid(QString filePath)
{
    return _grid.load(filePath);
}

const Grid * GameInfo::getGrid() const
{
    return &_grid;
}

void GameInfo::moveRight()
{
    int newX = _botPos.x + 1;
    int newY = _botPos.y;

    if (_grid.isPosAvailable(newX, newY))
    {
        _grid.move(_botPos.x, _botPos.y, newX, newY);
    }

    _botPos.x = newX;
    _botPos.y = newY;
}
