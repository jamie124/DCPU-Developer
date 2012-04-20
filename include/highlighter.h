#ifndef _HIGHLIGHTER_H
#define _HIGHLIGHTER_H

#include <QString>
#include <QVector>
#include <QSyntaxHighlighter>
#include <QRegExp>

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	Highlighter(QTextDocument *parent = 0);
	~Highlighter(void);

protected:
	void highlightBlock(const QString &text);

private:
	typedef struct {
		QRegExp pattern;
		QTextCharFormat format;
	} highlight_rule_t;

	QVector<highlight_rule_t> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormat;
	QTextCharFormat labelFormat;
	QTextCharFormat singleLineCommentFormat;
};

#endif
