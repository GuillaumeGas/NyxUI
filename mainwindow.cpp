#include "mainwindow.h"
#include "codeeditor.h"

#include <QScreen>
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

#include <compilo/Compilo.hpp>

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
    _outputTextEdit->setFontFamily("Courier New");

    _outputLayout->addWidget(_outputRenderer);
    _outputLayout->addWidget(_outputTextEdit);

    _editorTabWidget = new QTabWidget;
    _editorVBoxLayout = new QVBoxLayout;

    _scriptFileLabel = new QLabel(this);

    _editorTextEdit = new CodeEditor(this);
    _editorTextEdit->setFontFamily("Courier New");
    _executeButton = new QPushButton("Execute (F12)", this);

    _executeShortcut = new QShortcut(QKeySequence("F12"), this);
    _saveFileShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    _openScriptFileShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    _openMapFileShortcut = new QShortcut(QKeySequence("Ctrl+M"), this);

    connect(_executeButton, SIGNAL(clicked()), this, SLOT(executeScriptFile()));
    connect(_executeShortcut, SIGNAL(activated()), this, SLOT(executeScriptFile()));
    connect(_editorTextEdit, SIGNAL(textChanged()), this, SLOT(editorTextEditChanged()));
    connect(_saveFileShortcut, SIGNAL(activated()), this, SLOT(saveFile()));
    connect(_openScriptFileShortcut, SIGNAL(activated()), this, SLOT(openScriptFile()));
    connect(_openMapFileShortcut, SIGNAL(activated()), this, SLOT(openMapFile()));

    _editorVBoxLayout->addWidget(_scriptFileLabel);
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

    _actionMapFile = new QAction("Open map file...", this);
    _actionScriptFile = new QAction("Open script...", this);
    _actionQuit = new QAction("Quit", this);

    fileMenu->addAction(_actionMapFile);
    fileMenu->addAction(_actionScriptFile);
    fileMenu->addAction(_actionQuit);

    connect(_actionMapFile, SIGNAL(triggered()), this, SLOT(openMapFile()));
    connect(_actionScriptFile, SIGNAL(triggered()), this, SLOT(openScriptFile()));
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

void MainWindow::openScriptFile()
{
    QString scriptFile = QFileDialog::getOpenFileName(this, "Open script file", "", "*.nx");
    if (scriptFile.length() > 0)
    {
        QFile file(scriptFile);
        if (!file.exists())
        {
            QMessageBox::warning(this, "File", "File not found");
            return;
        }

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "File", "Cannot open file");
            file.close();
            return;
        }

        QString content = file.readAll();
        _editorTextEdit->setPlainText(content);
        _currentScriptFilePath = file.fileName();
        _scriptFileLabel->setText(_currentScriptFilePath);
        _scriptFileLabel->setStyleSheet("font-style: normal");

        file.close();
    }
}

void MainWindow::saveFile()
{
    if (_currentScriptFilePath.isEmpty())
        return;

    QFile file(_currentScriptFilePath);
    if (!file.exists())
        return;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "File", "Couldn't open destination file");
        file.close();
        return;
    }

    file.resize(0); // delete file content
    file.write(_editorTextEdit->toPlainText().toStdString().c_str());

    setScriptFileLabelAsModified(false);

    file.close();
}

void MainWindow::executeScriptFile()
{
    std::string fName = _currentScriptFilePath.toStdString();
    std::stringstream ss;
    nyx::Compilo c(fName, ss, true);
    c.compile();
    QString res(ss.str().c_str());
    _outputTextEdit->setPlainText(res);
}

void MainWindow::editorTextEditChanged()
{
    setScriptFileLabelAsModified(true);
}

void MainWindow::setScriptFileLabelAsModified(bool isModified)
{
    if (isModified)
    {
        _scriptFileLabel->setStyleSheet("font-style: italic");
        _scriptFileLabel->setText(_currentScriptFilePath + "*");
    }
    else
    {
        _scriptFileLabel->setStyleSheet("font-style: normal");
        _scriptFileLabel->setText(_currentScriptFilePath);
    }
}
