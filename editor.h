/**
DCPU Emulator.
Written by James Whitwell, 2012.

Custom Editor stuff

Started 7-Apr-2012 
*/
#ifndef _EDITOR_H
#define _EDITOR_H

#include <QTextEdit>

class QCompleter;

class Editor : public QTextEdit
{
	Q_OBJECT;

public:
	Editor(QWidget *parent = 0);
	~Editor(void);

	void setCompleter(QCompleter *c);
	QCompleter* completer() const;

protected:
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent *e);

private slots:
	void insertCompletion(const QString &completion);

private:
	QString textUnderCursor() const;

private:
	QCompleter *c;
};

#endif
