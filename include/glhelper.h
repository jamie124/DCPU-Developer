#ifndef _GLHELPER_H
#define _GLHELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>

class QPainter;
class QPaintEvent;

class GLHelper
{
public:
	GLHelper(void);
	~GLHelper(void);

	void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
	QBrush background;
	QBrush circleBrush;
	QFont textFont;
	QPen textPen;
};

#endif
