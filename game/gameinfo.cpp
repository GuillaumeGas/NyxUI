#include <QFile>
#include <QMessageBox>

#include "gameinfo.h"

GameInfo::GameInfo() : _botPos(0, 0), _targetPos(0, 0), _turnsLimit(0)
{

}

GameStatus GameInfo::loadGame(QString filePath)
{
    GameStatus status = _loadHeader(filePath);
    if (status != GameStatus::OK)
        return status;

    status = _grid.load(filePath);
    if (status != GameStatus::OK)
        return status;

    _grid.setCellTypeAtPos(_botPos.x, _botPos.y, Grid::CellType::PLAYER);

    return status;
}

GameStatus GameInfo::_loadHeader(QString filePath)
{
    QFile file(filePath);
    if (!file.exists())
    {
        return GameStatus::FILE_NOT_FOUND;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "File", "Cannot read file !");
        return GameStatus::CANNOT_READ_FILE;
    }

    QTextStream stream(&file);
    QString line;

    line = stream.readLine();
    if (line != "HEADER" || stream.atEnd())
    {
        QMessageBox::warning(nullptr, "File", "Header not found in map file");
        return GameStatus::HEADER_NOT_FOUND;
    }

    // reading bot start position
    GameStatus status;
    if ((status = _readPositionInHeader(stream, _botPos)) != GameStatus::OK)
    {
        QMessageBox::warning(nullptr, "File", "Invalid header (wrong bot position)");
        return status;
    }

    // reading target position
    if ((status = _readPositionInHeader(stream, _targetPos)) != GameStatus::OK)
    {
        QMessageBox::warning(nullptr, "File", "Invalid header (wrong target position)");
        return status;
    }

    if ((status = _readSingleIntInHeader(stream, _turnsLimit)) != GameStatus::OK)
    {
        QMessageBox::warning(nullptr, "File", "Invalid header (wrong turns limit)");
        return status;
    }

    file.close();

    return GameStatus::OK;
}

GameStatus GameInfo::_readPositionInHeader(QTextStream & stream, Position & pos)
{
    QString line = stream.readLine();
    QStringList splitLine = line.split(' ');
    if (splitLine.length() != 2)
        return GameStatus::HEADER_INVALID;

    pos.x = splitLine[0].toInt();
    pos.y = splitLine[1].toInt();

    return GameStatus::OK;
}

GameStatus GameInfo::_readSingleIntInHeader(QTextStream & stream, unsigned int & value)
{
    QString line = stream.readLine();
    bool ok = true;
    line.toUInt(&ok);
    if (!ok)
        return GameStatus::HEADER_INVALID;
    return GameStatus::OK;
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
