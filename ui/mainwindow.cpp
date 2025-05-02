#include "mainwindow.h"
#include "codeeditor.h"

#include <QScreen>
#include <QApplication>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>

#include <compilo/Compilo.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _initView();
    _initMenu();
    _initDefaultData();
}

MainWindow::~MainWindow()
{
    if (_gameInfo)
        delete _gameInfo;
    if (_nyxSyscallInterface)
        delete _nyxSyscallInterface;
}

void MainWindow::_initDefaultData()
{
    _nyxDebugMode = false;
    _nbOpennedFiles = 0;
    _gameInfo = new GameInfo();
    _nyxSyscallInterface = new NyxSyscallsInterface(_outputStream, _gameInfo);
}

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

    _executeShortcut = new QShortcut(QKeySequence("F12"), this);
    _saveFileShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    _openScriptFileShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    _openMapFileShortcut = new QShortcut(QKeySequence("Ctrl+M"), this);

    connect(_executeShortcut, SIGNAL(activated()), this, SLOT(executeScriptFile()));
    connect(_saveFileShortcut, SIGNAL(activated()), this, SLOT(saveFile()));
    connect(_openScriptFileShortcut, SIGNAL(activated()), this, SLOT(openScriptFile()));
    connect(_openMapFileShortcut, SIGNAL(activated()), this, SLOT(openMapFile()));

    _editorLayout->addWidget(_editorTabWidget);

    _mainLayout->addLayout(_outputLayout);
    _mainLayout->addLayout(_editorLayout);

    _window->setLayout(_mainLayout);
    setCentralWidget(_window);

    QRect rect = QGuiApplication::primaryScreen()->geometry();
    resize(rect.size() * 0.7);
}

void MainWindow::_initMenu()
{
    QMenu * fileMenu = menuBar()->addMenu("File");

    _actionMapFile = new QAction("Open map file...", this);
    _actionScriptFile = new QAction("Open script...", this);
    _actionNewScriptFile = new QAction("New script", this);
    _actionQuit = new QAction("Quit", this);

    fileMenu->addAction(_actionMapFile);
    fileMenu->addAction(_actionScriptFile);
    fileMenu->addAction(_actionNewScriptFile);
    fileMenu->addAction(_actionQuit);

    connect(_actionMapFile, SIGNAL(triggered()), this, SLOT(openMapFile()));
    connect(_actionScriptFile, SIGNAL(triggered()), this, SLOT(openScriptFile()));
    connect(_actionNewScriptFile, SIGNAL(triggered()), this, SLOT(newScriptFile()));
    connect(_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu * nyxMenu = menuBar()->addMenu("Nyx");
    QToolBar * nyxToolBar = addToolBar("Nyx");

    _actionNyxRun = new QAction("Run", this);

    _actionNyxDebugMode = new QAction("Ast", this);
    _actionNyxDebugMode->setCheckable(true);

    nyxMenu->addAction(_actionNyxRun);
    nyxMenu->addAction(_actionNyxDebugMode);
    nyxToolBar->addAction(_actionNyxRun);
    nyxToolBar->addAction(_actionNyxDebugMode);

    connect(_actionNyxRun, SIGNAL(triggered()), this, SLOT(executeScriptFile()));
    connect(_actionNyxDebugMode, SIGNAL(toggled(bool)), this, SLOT(nyxDebugMode(bool)));

    QMenu * gameMenu = menuBar()->addMenu("Game");
    QToolBar * gameToolBar = addToolBar("Game");

    _actionGameReset = new QAction("Reset", this);

    gameMenu->addAction(_actionGameReset);
    gameToolBar->addAction(_actionGameReset);

    connect(_actionGameReset, SIGNAL(triggered()), this, SLOT(gameReset()));
}

CodeEditor * MainWindow::_newCodeEditor()
{
    CodeEditor * editorTextEdit = new CodeEditor(this);
    editorTextEdit->setFontFamily("Courier New");
    return editorTextEdit;
}

QWidget * MainWindow::_newCodeEditorWidget()
{
    auto scriptFileLabel = new QLabel(this);
    _scriptFileLabelVector.append(scriptFileLabel);

    CodeEditor * codeEditor = _newCodeEditor();
    codeEditor->setFocus();
    _codeEditorVector.append(codeEditor);

    connect(codeEditor, SIGNAL(textChanged()), this, SLOT(editorTextEditChanged()));

    auto editorLayout = new QVBoxLayout;
    editorLayout->addWidget(scriptFileLabel);
    editorLayout->addWidget(codeEditor);

    auto tabWidget = new QWidget;
    tabWidget->setLayout(editorLayout);

    return tabWidget;
}

void MainWindow::openMapFile()
{
    QString mapFile = QFileDialog::getOpenFileName(this, "Open map file");
    if (mapFile.length() > 0)
    {
        _loadMapFile(mapFile);
    }
}

void MainWindow::_loadMapFile(QString mapFile)
{
    Grid::Status gridStatus = _gameInfo->loadGrid(mapFile);
    if (gridStatus == Grid::Status::OK)
    {
        _outputRenderer->updateGrid(_gameInfo->getGrid());
        _currentMapFile = mapFile;
    }
    else
    {
        switch(gridStatus)
        {
        case Grid::Status::FILE_NOT_FOUND:
            QMessageBox::warning(this, "File", "File not found !");
            break;
        case Grid::Status::CANNOT_READ_FILE:
            QMessageBox::warning(nullptr, "File", "Cannot read file !");
            break;
        }
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

        _filesPathVector.append(file.fileName());

        QString content = file.readAll();

        CodeEditor * currentCodeEditor = nullptr;
        QLabel * label = nullptr;

        _editorTabWidget->addTab(_newCodeEditorWidget(), "Editor");
        currentCodeEditor = _codeEditorVector.last();
        label = _scriptFileLabelVector.last();

        currentCodeEditor->setPlainText(content);

        QString scriptFilePath = file.fileName();
        label->setText(scriptFilePath);
        label->setStyleSheet("font-style: normal");

        QFileInfo fileInfo(file.fileName());
        _editorTabWidget->setCurrentIndex(_editorTabWidget->count() - 1);
        _editorTabWidget->setTabText(_editorTabWidget->currentIndex(), fileInfo.fileName());

        _nbOpennedFiles++;

        file.close();
    }
}

void MainWindow::newScriptFile()
{
    _editorTabWidget->addTab(_newCodeEditorWidget(), "Editor");
    _filesPathVector.push_back("");
    _nbOpennedFiles++;
}

void MainWindow::saveFile()
{
    if (_nbOpennedFiles == 0)
        return;

    unsigned int indexFilePath = _editorTabWidget->currentIndex();

    QString & filePath = _filesPathVector[indexFilePath];

    if (filePath == "")
    {
        filePath = _askForNewFilePath();
        _filesPathVector[indexFilePath];
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "File", "Couldn't open destination file");
        file.close();
        return;
    }

    CodeEditor * currentCodeEditor = _codeEditorVector[_editorTabWidget->currentIndex()];
    QLabel * currentFileLabel = _scriptFileLabelVector[_editorTabWidget->currentIndex()];

    file.resize(0); // delete file content
    file.write(currentCodeEditor->toPlainText().toStdString().c_str());

    _setScriptFileLabelAsModified(false, currentFileLabel, filePath);

    file.close();
}

QString MainWindow::_askForNewFilePath()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    return dialog.getSaveFileName(this, "New script file");
}

void MainWindow::executeScriptFile()
{
    if (_nbOpennedFiles == 0)
        return;

    QString & filePath = _filesPathVector[_editorTabWidget->currentIndex()];
    std::string fName = filePath.toStdString();

    _outputStream.clear();
    _outputStream.str(string());

    nyx::Compilo c(fName, _nyxSyscallInterface, _outputStream, _nyxDebugMode);
    c.compile();

    QString res(_outputStream.str().c_str());

    _outputTextEdit->clear();
    _outputTextEdit->setPlainText(res);
    _outputRenderer->updateGrid(_gameInfo->getGrid());
    _outputRenderer->repaint();
}

void MainWindow::editorTextEditChanged()
{
    QLabel * currentFileLabel = _scriptFileLabelVector[_editorTabWidget->currentIndex()];
    QString & filePath = _filesPathVector[_editorTabWidget->currentIndex()];
    _setScriptFileLabelAsModified(true, currentFileLabel, filePath);
}

void MainWindow::_setScriptFileLabelAsModified(bool isModified, QLabel * scriptFileLabel, QString & scriptFilePath)
{
    if (_nbOpennedFiles == 0)
        return;

    if (isModified)
    {
        scriptFileLabel->setStyleSheet("font-style: italic");
        scriptFileLabel->setText(scriptFilePath + "*");
    }
    else
    {
        scriptFileLabel->setStyleSheet("font-style: normal");
        scriptFileLabel->setText(scriptFilePath);
    }
}

void MainWindow::nyxDebugMode(bool checked)
{
    _nyxDebugMode = checked;
}

void MainWindow::gameReset()
{
    _loadMapFile(_currentMapFile);
    _outputRenderer->repaint();
    _outputTextEdit->clear();
}
