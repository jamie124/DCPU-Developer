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

signals:
	void addToCodeComplete(QString);

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
	
	QTextCharFormat regularText;
	QTextCharFormat keywordFormat;

	QTextCharFormat labelFormat;
	QTextCharFormat labelRefFormat;

	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat registerFormat;
	QTextCharFormat datStringFormat;
	QTextCharFormat numericHexFormat;

	QTextCharFormat specialOperationFormat;

};

#endif
