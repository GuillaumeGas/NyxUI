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
