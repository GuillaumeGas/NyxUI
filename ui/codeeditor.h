#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>

#include "highlighter.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget * parent = nullptr);

    int lineNumberWidgetWidth();
    void lineNumberWidgetPaintEvent(QPaintEvent * event);

    void setFontFamily(QString fontFamily);

protected:
    void resizeEvent(QResizeEvent * event) override;

private slots:
    void updateLineNumberWidgetWidth(int newBlockCount);
    void updateLineNumberWidget(const QRect & rect, int nbPixelsScrolled);
    void highlightCurrentLine();

private:
    void setTabSize();

    QWidget * lineNumberWidget;

    Highlighter * highlighter;
};

#endif // CODEEDITOR_H
