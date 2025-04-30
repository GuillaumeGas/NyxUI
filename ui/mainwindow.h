#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>

#include <sstream>

#include "renderer.h"
#include "codeeditor.h"
#include "../nyx/nyxsyscallsinterface.h"
#include "../game/gameinfo.h"

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
    void editorTextEditChanged();
    void saveFile();
    void nyxDebugMode(bool checked);

private:
    QWidget * _window;

    QHBoxLayout * _mainLayout;
    QVBoxLayout * _outputLayout;
    QVBoxLayout * _editorLayout;

    Renderer * _outputRenderer;
    QTextEdit * _outputTextEdit;

    QTabWidget * _editorTabWidget;
    QVector<QLabel*> _scriptFileLabelVector;
    QVector<CodeEditor*> _codeEditorVector;
    QVector<QString> _filesPathVector;

    QShortcut * _executeShortcut;
    QShortcut * _saveFileShortcut;
    QShortcut * _openScriptFileShortcut;
    QShortcut * _openMapFileShortcut;

    QAction * _actionMapFile;
    QAction * _actionScriptFile;
    QAction * _actionQuit;

    QAction * _actionNyxRun;
    QAction * _actionNyxDebugMode;

    bool _nyxDebugMode;
    int _nbOpennedFiles;
    std::stringstream _outputStream;
    NyxSyscallsInterface * _nyxSyscallInterface;
    GameInfo * _gameInfo;

    void _initView();
    void _initMenu();
    void _initDefaultData();

    void _setScriptFileLabelAsModified(bool isModified, QLabel * fileLabel, QString & scriptFilePath);
    CodeEditor * _newCodeEditor();
    QWidget * _newCodeEditorWidget();
};
#endif // MAINWINDOW_H
