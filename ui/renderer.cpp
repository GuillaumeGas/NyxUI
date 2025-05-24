#include "renderer.h"

#include <QPainter>
#include <QFrame>

#define GRID_SIZE 200
#define CELL_SIZE 20

Renderer::Renderer(QWidget *parent)
    : QWidget{parent}
{
}

void Renderer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    drawMapAndPlayers(painter);
    drawGrid(painter);
}

void Renderer::drawMapAndPlayers(QPainter & painter)
{
    QPixmap wallImage(":/images/wall.png");
    QPixmap emptyImage(":/images/empty.png");
    QPixmap aiImage(":images/ai.png");
    QPixmap targetImage(":images/target.png");

    const std::vector<std::vector<int>> & grid = _grid.getGrid();

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid.size(); j++)
        {
            int x = (j * CELL_SIZE) + (width() - (GRID_SIZE + CELL_SIZE)) / 2;
            int y = (i * CELL_SIZE) + (height() - (GRID_SIZE + CELL_SIZE)) / 2;

            switch(grid[i][j])
            {
            case 0:
                painter.drawPixmap(x, y, emptyImage);
                break;
            case 1:
                painter.drawPixmap(x, y, wallImage);
                break;
            case 2:
                painter.drawPixmap(x, y, aiImage);
                break;
            case 3:
                painter.drawPixmap(x, y, targetImage);
                break;
            default:
                painter.drawPixmap(x, y, emptyImage);
            }
        }
    }
}

void Renderer::drawGrid(QPainter & painter)
{
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    QList<QLine> verticalLines;
    QList<QLine> horizontalLines;

    for (int i = 0; i <= GRID_SIZE; i += CELL_SIZE)
    {
        int x = i + (width() - (GRID_SIZE + CELL_SIZE)) / 2;
        int y = (height() - (GRID_SIZE + CELL_SIZE)) / 2;

        QPoint p1(x, y);
        QPoint p2(x, y + GRID_SIZE);
        verticalLines.append(QLine(p1, p2));
    }

    for (int i = 0; i <= GRID_SIZE; i += CELL_SIZE)
    {
        int y = i + (height() - (GRID_SIZE + CELL_SIZE)) / 2;
        int x = (width() - (GRID_SIZE + CELL_SIZE)) / 2;

        QPoint p1(x, y);
        QPoint p2(x + GRID_SIZE, y);
        horizontalLines.append(QLine(p1, p2));
    }

    painter.drawLines(verticalLines);
    painter.drawLines(horizontalLines);
}

QSize Renderer::minimumSizeHint() const
{
    return QSize(400, 400);
}

QSize Renderer::sizeHint() const
{
    return QSize(400, 400);
}

void Renderer::updateGrid(const Grid * grid)
{
    _grid = Grid(*grid);
}
