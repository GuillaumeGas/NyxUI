#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>

#include "../game/grid.h"

#define OUTPUT_RENDERER_GRID_SIZE 10

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void updateGrid(const Grid * grid);

signals:

private:
    virtual void paintEvent(QPaintEvent*) override;
    void drawMapAndPlayers(QPainter & painter);
    void drawGrid(QPainter & painter);

    Grid _grid;
};

#endif // RENDERER_H
