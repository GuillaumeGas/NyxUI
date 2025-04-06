#ifndef GRID_H
#define GRID_H

#include <QString>
#include <QVector>

class Grid
{
public:
    Grid();

    void load(QString filePath);
    const QVector<QVector<int>> & getGrid() const;
    int getSize() const;

private:
    QVector<QVector<int>> _grid;
};

#endif // GRID_H
