#include "linenumberwidget.h"

LineNumberWidget::LineNumberWidget(CodeEditor * codeEditor)
    : QWidget(codeEditor), codeEditor(codeEditor)
{}

QSize LineNumberWidget::sizeHint() const
{
    return QSize(codeEditor->lineNumberWidgetWidth(), 0);
}

void LineNumberWidget::paintEvent(QPaintEvent * event)
{
    codeEditor->lineNumberWidgetPaintEvent(event);
}
