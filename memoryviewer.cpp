#include <QtGui>
#include <QDebug>

#include "include/memoryviewer.h"
#include "include/glhelper.h"

MemoryViewer::MemoryViewer(GLHelper *glHelper, QWidget *parent) 
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent), glHelper(glHelper)
{
	elapsed = 0;
	setFixedSize(200, 200);
	setAutoFillBackground(false);
}

MemoryViewer::~MemoryViewer(void)
{
	memoryMap.clear();
}

void MemoryViewer::setMemoryMap(word_map map) 
{
	memoryMap = map;
}

void MemoryViewer::animate()
{
	elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
	repaint();
}

void MemoryViewer::paintEvent(QPaintEvent *event)
{
	QPainter painter;
	
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	glHelper->paint(&painter, memoryMap, event, elapsed);
	painter.end();
}
