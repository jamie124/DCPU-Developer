#include <QtGui>

#include "include/glhelper.h"

GLHelper::GLHelper()
{
	QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
	gradient.setColorAt(0.0, Qt::white);
	gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

	background = QBrush(QColor(64, 32, 64));
	circleBrush = QBrush(gradient);
	textPen = QPen(Qt::white);
	textFont.setPixelSize(50);
}


GLHelper::~GLHelper()
{
}

void GLHelper::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
	painter->fillRect(event->rect(), background);
	painter->translate(100, 100);

//	painter->save();
	
	painter->setPen(textPen);
	painter->setFont(textFont);
	painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, "Test");
}