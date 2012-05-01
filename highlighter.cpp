#include <QDebug>
#include "include/highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
	highlight_rule_t rule;

	keywordFormat.setForeground(Qt::blue);
	keywordFormat.setFontWeight(QFont::Bold);

	// Keywords
	QStringList keywordPatterns;
	keywordPatterns 
		// Basic Set, Add, Sub
		<< "\\bset\\b" << "\\badd\\b" << "\\bsub\\b"
		// Multi / Divide / Mod
		<< "\\bmul\\b" << "\\bmli\\b" << "\\bdiv\\b"  << "\\bdvi\\b" 
		<< "\\bmod\\b" << "\\bmdi\\b"
		// Binary Operations
		<< "\\band\\b" << "\\bbor\\b" << "\\bxor\\b"
		<< "\\bshr\\b" << "\\basr\\b" << "\\bshl\\b" 
		// If's
		<< "\\bifb\\b" << "\\bifc\\b" << "\\bife\\b" << "\\bifn\\b" << "\\bifg\\b" 
		<< "\\bifa\\b" << "\\bifl\\b" << "\\bifu\\b"
		// Overflow
		<< "\\badx\\b" << "\\bsbx\\b"
		// Set with Inc / Dec
		<< "\\bsti\\b" << "\\bstd\\b"
		<< "\\bdat\\b";

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

	// Register
	registerFormat.setForeground(Qt::darkRed);
	rule.pattern = QRegExp("[ a|b|c|x|y|z|i|j|pc|sp]+,");
	rule.format = registerFormat;
	highlightingRules.append(rule);

	// Dat String
	datStringFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("\"[a-zA-Z1-9!. ]+\"");
	rule.format = datStringFormat;
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

			if (text.contains(QRegExp(":[^ ]*"))) {
				if (text.contains(" ")) {
					qDebug() << "Adding " << text.mid(text.indexOf(":") + 1, length - 1);
					emit addToCodeComplete(text.mid(text.indexOf(":") + 1, length - 1), false);
				} else {
					qDebug() << "Removing " << text.mid(text.indexOf(":") + 1, length - 1);
					emit addToCodeComplete(text.mid(text.indexOf(":") + 1, length - 1), true);
				}
			}
			index = expression.indexIn(text, index + length);
		}
	}
}