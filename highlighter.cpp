#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	highlight_rule_t rule;

	keywordFormat.setForeground(Qt::blue);
	keywordFormat.setFontWeight(QFont::Bold);

	// Keywords
	QStringList keywordPatterns;
	keywordPatterns << "\\bset\\b" << "\\badd\\b" << "\\bsub\\b"
		<< "\\bmul\\b" << "\\bdiv\\b" << "\\bmod\\b"
		<< "\\bshl\\b" << "\\bshr\\b" << "\\band\\b"
		<< "\\bbor\\b" << "\\bxor\\b" << "\\bife\\b"
		<< "\\bifn\\b" << "\\bifg\\b" << "\\bifb\\b" << "\\bdat\\b";

	foreach (const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	// Label
	labelFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp(":[^ ]*");
	rule.format = labelFormat;
	highlightingRules.append(rule);

	// Comment
	singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp(";[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);
}


Highlighter::~Highlighter(void)
{
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach (const highlight_rule_t &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);

		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
}