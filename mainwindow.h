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
    void openMapFile();
    void openScriptFile();
    void executeScriptFile();

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

    QAction * _actionMapFile;
    QAction * _actionScriptFile;
    QAction * _actionQuit;

    QString _currentScriptFilePath;

    void _initView();
    void _initMenu();
};
#endif // MAINWINDOW_H
