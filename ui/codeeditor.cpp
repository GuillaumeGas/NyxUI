#include <QPainter>
#include <QTextBlock>

#include "codeeditor.h"
#include "linenumberwidget.h"

CodeEditor::CodeEditor(QWidget * parent) : QPlainTextEdit(parent)
{
    lineNumberWidget = new LineNumberWidget(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberWidgetWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberWidget);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    highlighter = new Highlighter(document());

    updateLineNumberWidgetWidth(0);
    highlightCurrentLine();
    setTabSize();
}

void CodeEditor::setFontFamily(QString fontFamily)
{
    QTextDocument * doc = document();
    QFont font = doc->defaultFont();
    font.setFamily(fontFamily);

    setTabStopDistance(QFontMetricsF(font).horizontalAdvance(' ') * 4);

    doc->setDefaultFont(font);
}

void CodeEditor::setTabSize()
{
    QTextDocument * doc = document();
    QFont font = doc->defaultFont();

    setTabStopDistance(QFontMetricsF(font).horizontalAdvance(' ') * 4);
}

int CodeEditor::lineNumberWidgetWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberWidgetWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberWidgetWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberWidget(const QRect &rect, int nbPixelsScrolled)
{
    if (nbPixelsScrolled > 0)
    {
        lineNumberWidget->scroll(0, nbPixelsScrolled);
    }
    else
    {
        lineNumberWidget->update(0, rect.y(), lineNumberWidget->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberWidgetWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent * e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberWidget->setGeometry(QRect(cr.left(), cr.top(), lineNumberWidgetWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::green).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberWidgetPaintEvent(QPaintEvent * event)
{
    QPainter painter(lineNumberWidget);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberWidget->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
