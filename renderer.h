#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>

#include "grid.h"

#define OUTPUT_RENDERER_GRID_SIZE 10

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void updateGrid(QString filePath);

signals:

private:
    virtual void paintEvent(QPaintEvent*) override;
    void drawMapAndPlayers(QPainter & painter);
    void drawGrid(QPainter & painter);

    Grid _grid;
};

#endif // RENDERER_H
