#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>

#include "renderer.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();

private:
    QWidget * _window;

    QHBoxLayout * _mainLayout;
    QVBoxLayout * _outputLayout;
    QVBoxLayout * _editorLayout;

    Renderer * _outputRenderer;
    QTextEdit * _outputTextEdit;

    QTabWidget * _editorTabWidget;
    QVBoxLayout * _editorVBoxLayout;
    QTextEdit * _editorTextEdit;
    QPushButton * _executeButton;

    QAction * _actionFile;
    QAction * _actionQuit;

    void _initView();
    void _initMenu();
    void _updateOutputRenderer(int grid[OUTPUT_RENDERER_GRID_SIZE][OUTPUT_RENDERER_GRID_SIZE]);
};
#endif // MAINWINDOW_H
