#ifndef RENDERER_H
#define RENDERER_H

#include <QWidget>

#define OUTPUT_RENDERER_GRID_SIZE 10

class Renderer : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void updateGrid(const int grid[OUTPUT_RENDERER_GRID_SIZE][OUTPUT_RENDERER_GRID_SIZE]);

signals:

private:
    virtual void paintEvent(QPaintEvent*) override;
};

#endif // RENDERER_H
