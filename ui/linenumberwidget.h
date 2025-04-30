#ifndef LINENUMBERWIDGET_H
#define LINENUMBERWIDGET_H

#include <QObject>

#include "codeeditor.h"

class LineNumberWidget : public QWidget
{
public:
    explicit LineNumberWidget(CodeEditor * editor);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent * event);

private:
    CodeEditor * codeEditor;

signals:
};

#endif // LINENUMBERWIDGET_H
