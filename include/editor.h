/**
DCPU Emulator.
Written by James Whitwell, 2012.

Custom Editor stuff

Started 7-Apr-2012 
*/
#ifndef _EDITOR_H
#define _EDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QPainter>
#include <QTextBlock>

class QCompleter;
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

class LineNumberArea;

class Editor : public QPlainTextEdit
{
	Q_OBJECT;

public:
	Editor(QWidget *parent = 0);
	~Editor(void);

	void setCompleter(QCompleter *c);
	QCompleter* completer() const;

	 void lineNumberAreaPaintEvent(QPaintEvent *event);
     int lineNumberAreaWidth();

	 void setLine(int lineNumber);

	 void formatCode();

protected:
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent *e);
    void resizeEvent(QResizeEvent *event);

private slots:
	void insertCompletion(const QString &completion);
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
	QString textUnderCursor() const;

private:
	QCompleter *c;

	QWidget *lineNumberArea;
};

// Line Number Area
class LineNumberArea : public QWidget
 {
 public:
     LineNumberArea(Editor *editor) : QWidget(editor) {
         codeEditor = editor;
     }

     QSize sizeHint() const {
         return QSize(codeEditor->lineNumberAreaWidth(), 0);
     }

 protected:
     void paintEvent(QPaintEvent *event) {
         codeEditor->lineNumberAreaPaintEvent(event);
     }

 private:
     Editor *codeEditor;
 };

#endif
