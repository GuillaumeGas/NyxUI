#include "grid.h"

#include <QFile>
#include <QMessageBox>

Grid::Grid() {}

GameStatus Grid::load(QString filePath)
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

    _grid.clear();

    int nbLinesRef = 0;
    int nbColumnsRef = 0;
    bool invalidGridDim = false;

    QTextStream stream(&file);
    QString line;

    while (!stream.atEnd() && ((line = stream.readLine()) != "MAP")) {}

    int lineIndex = 0;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        int nbColumns = line.length();

        _grid.push_back(std::vector<int>(nbColumns));

        for (int i = 0; i < nbColumns; i++)
        {
            _grid[lineIndex][i] = line[i].digitValue();
        }

        if (nbColumnsRef == 0)
        {
            nbColumnsRef = nbColumns;
        }
        else if (nbColumnsRef != nbColumns)
        {
            invalidGridDim = true;
        }

        nbLinesRef++;
        lineIndex++;
    }

    if (nbColumnsRef != nbLinesRef)
    {
        invalidGridDim = true;
    }

    if (invalidGridDim)
    {
        _grid.clear();
        QMessageBox::critical(nullptr, "Grid", "Invalid grid dimension");
    }

    file.close();

    return GameStatus::OK;
}

const std::vector<std::vector<int>> & Grid::getGrid() const
{
    return _grid;
}

int Grid::getSize() const
{
    return _grid.size();
}

bool Grid::isPosAvailable(int x, int y)
{
    if (x >= _grid.size() || y >= _grid.size())
        return false;

    return _grid[y][x] == CellType::EMPTY || _grid[y][x] == CellType::TARGET;
}

void Grid::move(int x, int y, int newX, int newY)
{
    if (x >= _grid.size() || y >= _grid.size())
        return;
    if (newX >= _grid.size() || newY >= _grid.size())
        return;

    _grid[newY][newX] = _grid[y][x];
    _grid[y][x] = 0;
}

void Grid::setCellTypeAtPos(int x, int y, CellType type)
{
    if (x >= _grid.size() || y >= _grid.size())
        return;

    _grid[y][x] = type;
}
