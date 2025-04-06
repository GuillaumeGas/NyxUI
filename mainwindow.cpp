#include "mainwindow.h"

#include <QScreen>
#include <QApplication>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _initView();
    _initMenu();

    // TEST
    int grid[OUTPUT_RENDERER_GRID_SIZE][OUTPUT_RENDERER_GRID_SIZE];
    for (int i = 0; i < OUTPUT_RENDERER_GRID_SIZE; i++)
        for (int j = 0; j < OUTPUT_RENDERER_GRID_SIZE; j++)
            grid[i][j] = 0;
    grid[0][0] = 1;
    _updateOutputRenderer(grid);
}

MainWindow::~MainWindow() {}

void MainWindow::_initView()
{
    _window = new QWidget;

    _mainLayout = new QHBoxLayout;
    _outputLayout = new QVBoxLayout;
    _editorLayout = new QVBoxLayout;

    _outputRenderer = new Renderer;
    _outputTextEdit = new QTextEdit;

    _outputLayout->addWidget(_outputRenderer);
    _outputLayout->addWidget(_outputTextEdit);

    _editorTabWidget = new QTabWidget;
    _editorVBoxLayout = new QVBoxLayout;
    _editorTextEdit = new QTextEdit;
    _executeButton = new QPushButton("Execute (F12)");

    _editorVBoxLayout->addWidget(_editorTextEdit);
    _editorVBoxLayout->addWidget(_executeButton);

    auto tabWidget = new QWidget;
    tabWidget->setLayout(_editorVBoxLayout);
    _editorTabWidget->addTab(tabWidget, "Editor");

    _editorLayout->addWidget(_editorTabWidget);

    _mainLayout->addLayout(_outputLayout);
    _mainLayout->addLayout(_editorLayout);

    _window->setLayout(_mainLayout);
    setCentralWidget(_window);

    _editorTextEdit->setFocus();

    QRect rect = QGuiApplication::primaryScreen()->geometry();
    resize(rect.size() * 0.7);
}

void MainWindow::_initMenu()
{
    QMenu * fileMenu = menuBar()->addMenu("File");

    _actionFile = new QAction("Open file...", this);
    _actionQuit = new QAction("Quit", this);

    fileMenu->addAction(_actionFile);
    fileMenu->addAction(_actionQuit);

    connect(_actionFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::openFile()
{

}

void MainWindow::_updateOutputRenderer(int grid[OUTPUT_RENDERER_GRID_SIZE][OUTPUT_RENDERER_GRID_SIZE])
{
    _outputRenderer->updateGrid("C:\\Users\\Guillaume\\Documents\\prog\\c++\\NyxUI\\map");
}
