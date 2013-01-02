#include "lemviewer.h"
#include <QtGui>

LemViewer::LemViewer(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	setFixedSize(200, 200);
	setAutoFillBackground(false);
}


LemViewer::~LemViewer()
{
}

void LemViewer::animate()
{
	elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
	repaint();
}

void LemViewer::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	//glHelper->paint(&painter, memoryMap, event, elapsed);
	painter.end();
}
