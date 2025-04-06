#include "mainwindow.h"

#include <QScreen>
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _initView();
    _initMenu();
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

    _actionFile = new QAction("Open map file...", this);
    _actionQuit = new QAction("Quit", this);

    fileMenu->addAction(_actionFile);
    fileMenu->addAction(_actionQuit);

    connect(_actionFile, SIGNAL(triggered()), this, SLOT(openMapFile()));
    connect(_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::openMapFile()
{
    QString mapFile = QFileDialog::getOpenFileName(this, "Open map file");
    if (mapFile.length() > 0)
    {
        _outputRenderer->updateGrid(mapFile);
    }
}
