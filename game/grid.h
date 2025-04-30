#ifndef GRID_H
#define GRID_H

#include <QString>
#include <vector>

class Grid
{
public:
    enum Status
    {
        OK,
        FILE_NOT_FOUND,
        CANNOT_READ_FILE,
    };

    Grid();

    Status load(QString filePath);
    const std::vector<std::vector<int>> & getGrid() const;
    int getSize() const;

    bool isPosAvailable(int x, int y);
    void move(int x, int y, int newX, int newY);

private:
    std::vector<std::vector<int>> _grid;
};

#endif // GRID_H
