#include "grid.h"

#include <QFile>
#include <QMessageBox>

Grid::Grid() {}

void Grid::load(QString filePath)
{
    QFile file(filePath);
    if (!file.exists())
    {
        QMessageBox::warning(nullptr, "File", "File not found !");
        return;
    }

    QString line;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "File", "Cannot read file !");
        return;
    }

    _grid.clear();

    int nbLinesRef = 0;
    int nbColumnsRef = 0;
    bool invalidGridDim = false;

    QTextStream stream(&file);
    int lineIndex = 0;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        int nbColumns = line.length();

        _grid.push_back(QVector<int>(nbColumns));

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
    else
    {
        QMessageBox::information(nullptr, "File", "File loaded successfully !");
    }
}

const QVector<QVector<int>> & Grid::getGrid() const
{
    return _grid;
}

int Grid::getSize() const
{
    return _grid.count();
}
