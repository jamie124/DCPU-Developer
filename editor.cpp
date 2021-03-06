#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

#include "include/editor.h"

Editor::Editor(QWidget *parent)
	: QPlainTextEdit(parent), c(0)
{
	lineNumberArea = new LineNumberArea(this);

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();


	QPalette palette = this->palette();

	palette.setColor(QPalette::Active, QPalette::Base, Qt::black);
	palette.setColor(QPalette::Inactive, QPalette::Base, Qt::black);

	this->setPalette(palette);

	this->setTabStopWidth(40);
}

Editor::~Editor()
{
}

void Editor::setCompleter(QCompleter *completer)
{
	if (c) {
		QObject::disconnect(c, 0, this, 0);
	}

	c = completer;

	if (!c) {
		return;
	}

	c->setWidget(this);
	c->setCompletionMode(QCompleter::PopupCompletion);
	c->setCaseSensitivity(Qt::CaseInsensitive);
	connect(c, SIGNAL(activated(QString)),
		this, SLOT(insertCompletion(QString)));
}

QCompleter *Editor::completer() const
{
	return c;
}

void Editor::insertCompletion(const QString& completion)
{
	if (c->widget() != this) {
		return;
	}

	QTextCursor tc = textCursor();
	int extra = completion.length() - c->completionPrefix().length();
	tc.movePosition(QTextCursor::Left);
	tc.movePosition(QTextCursor::EndOfWord);
	tc.insertText(completion.right(extra));
	setTextCursor(tc);
}

QString Editor::textUnderCursor() const
{
	QTextCursor tc = textCursor();
	tc.select(QTextCursor::WordUnderCursor);
	return tc.selectedText();
}

void Editor::focusInEvent(QFocusEvent *e)
{
	if (c)
		c->setWidget(this);
	QPlainTextEdit::focusInEvent(e);
}

void Editor::keyPressEvent(QKeyEvent *e)
{
	if (c && c->popup()->isVisible()) {
		// The following keys are forwarded by the completer to the widget
		switch (e->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
			e->ignore();
			return; // let the completer do default behavior
		default:
			break;
		}
	}

	bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+E
	if (!c || !isShortcut) // do not process the shortcut when we have a completer
		QPlainTextEdit::keyPressEvent(e);

	const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
	if (!c || (ctrlOrShift && e->text().isEmpty()))
		return;

	static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
	bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
	QString completionPrefix = textUnderCursor();

	if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
		|| eow.contains(e->text().right(1)))) {
			c->popup()->hide();
			return;
	}

	if (completionPrefix != c->completionPrefix()) {
		c->setCompletionPrefix(completionPrefix);
		c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
	}

	QRect cr = cursorRect();

	cr.setWidth(c->popup()->sizeHintForColumn(0)
		+ c->popup()->verticalScrollBar()->sizeHint().width());

	c->complete(cr); // popup it up!
}

int Editor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void Editor::setLine(int lineNumber) {

	if (lineNumber > 0) {
		//qDebug() << QString::number(lineNumber);

		QTextCursor editorCursor = this->textCursor();
		editorCursor.setPosition(0);
		editorCursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, lineNumber-1)	;

		this->setTextCursor(editorCursor);
	}
}

void Editor::formatCode() {

	QString unformattedText = this->toPlainText();

	QString formattedText = "";

	QStringList splitStrings = unformattedText.split(QRegExp("[\r\n]"));

	QStringList::iterator itr;

	bool formattingInLabel = false;

	QString currentLine;

	QRegExp tabs("\t");

	for (itr = splitStrings.begin(); itr != splitStrings.end(); ++itr) {

		currentLine = *itr;

		if (currentLine != "") {

			// Remove shit from start and end of line
			currentLine = currentLine.trimmed();

			if (currentLine.contains(":")) {
				formattingInLabel = true;
			} else {
				currentLine = (formattingInLabel ? "\t" : "") + currentLine;
			}

			formattedText += currentLine + "\n";
		} else {
			//formattingInLabel = false;

			formattedText += "\n";
		}

	}

	//qDebug() << formattedText;

	this->setPlainText(formattedText);
}

void Editor::updateLineNumberAreaWidth(int)
{
	setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
	if (dy) {
		lineNumberArea->scroll(0, dy);
	} else {
		lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
	}

	if (rect.contains(viewport()->rect())) {
		updateLineNumberAreaWidth(0);
	}
}



void Editor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void Editor::highlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) {
		QTextEdit::ExtraSelection selection;

		QColor lineColor = QColor(QColor(46, 46, 46));

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}



void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(lineNumberArea);
	painter.fillRect(event->rect(), Qt::lightGray);


	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int) blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int) blockBoundingRect(block).height();
		++blockNumber;
	}
}