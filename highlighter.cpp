#include <QDebug>
#include "include/highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {

	highlight_rule_t rule;

	// Regular text
	regularText.setForeground(Qt::white);

	rule.pattern = QRegExp(".");
	rule.format = regularText;
	highlightingRules.append(rule);

	// Register
	registerFormat.setForeground(QColor(252, 53, 53));
	rule.pattern = QRegExp(" [a|b|c|x|y|z|i|j|pc|sp|ex|A|B|C|X|Y|Z|I|J|PC|SP|EX]+");
	rule.format = registerFormat;
	highlightingRules.append(rule);

	keywordFormat.setForeground(QColor(0, 149, 255));
	keywordFormat.setFontWeight(QFont::Bold);

	// Keywords
	QStringList keywordPatterns;
	keywordPatterns 
		// Basic Set, Add, Sub
		<< "set " << "add " << "sub "
		// Multi / Divide / Mod
		<< "mul " << "mli " << "div "  << "dvi " 
		<< "mod " << "mdi "
		// Binary Operations
		<< "and " << "bor " << "xor "
		<< "shr " << "asr " << "shl " 
		// If's
		<< "ifb " << "ifc " << "ife " << "ifn " << "ifg " 
		<< "ifa " << "ifl " << "ifu "
		// Overflow
		<< "adx " << "sbx "
		// Set with Inc / Dec
		<< "sti " << "std "
		<< "dat "
		// Specials
		<< "jsr " << "int " << "iag " << "ias " << "rfi " << "iaq " << "hwn " << "hwq " << "hwi ";

	foreach (const QString &pattern, keywordPatterns) {
		QString test = "(" + pattern + ")|(" + pattern.toUpper() + ")";
		rule.pattern = QRegExp(test);
		rule.format = keywordFormat;
		highlightingRules.append(rule);
	}

	// Label Reference
	labelRefFormat.setForeground(QColor(255, 162, 0));
	labelRefFormat.setFontWeight(QFont::Bold);

//	rule.pattern = QRegExp("[\[a-z_\"]+");
	rule.pattern = QRegExp("(|[ ])[a-z_\\[\\]+]");
	rule.format = labelRefFormat;
	highlightingRules.append(rule);

	// Label
	labelFormat.setForeground(QColor(255, 255, 0));
	labelFormat.setFontWeight(QFont::Bold);

	rule.pattern = QRegExp(":[^ ]*");
	rule.format = labelFormat;
	highlightingRules.append(rule);



	// Dat String
	datStringFormat.setForeground(QColor(0, 195, 255));
	rule.pattern = QRegExp("\"[a-zA-Z1-9!. ]+\"");
	rule.format = datStringFormat;
	highlightingRules.append(rule);

	// Numeric/Hex value
	numericHexFormat.setForeground(QColor(0, 195, 255));
	rule.pattern = QRegExp("(0x[0-9a-fA-F]+)|[0-9]+");
	rule.format = numericHexFormat;
	highlightingRules.append(rule);

	// Comment
	singleLineCommentFormat.setForeground(QColor(0, 255, 94));
	rule.pattern = QRegExp(";[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	// Special Operation, PEEK, PUSH, POP
	specialOperationFormat.setForeground(QColor(205, 54, 255));
	rule.pattern = QRegExp("(pop|push|pick|POP|PUSH|PICK)");
	rule.format = specialOperationFormat;
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

				emit addToCodeComplete(text.mid(text.indexOf(":") + 1, length - 1));
				/*
				if (text.contains(" ")) {
					//qDebug() << "Adding " << text.mid(text.indexOf(":") + 1, length - 1);
					emit addToCodeComplete(text.mid(text.indexOf(":") + 1, length - 1), false);
				} else {
					//qDebug() << "Removing " << text.mid(text.indexOf(":") + 1, length - 1);
					emit addToCodeComplete(text.mid(text.indexOf(":") + 1, length - 1), true);
				}
				*/
			}
			index = expression.indexIn(text, index + length);
		}
	}
}