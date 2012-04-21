#ifndef _MEMORYVIEWER_H
#define _MEMORYVIEWER_H

#include <QGLWidget>

class GLHelper;
class QPaintEvent;
class QWidget;

class MemoryViewer : public QGLWidget 
{
	Q_OBJECT

public:
	MemoryViewer(GLHelper *glHelper, QWidget *parent = 0);
	~MemoryViewer();

	void setMemoryMap(QMap<int, int> map);

public slots:
	void animate();

protected:
	void paintEvent(QPaintEvent *event);

private:
	GLHelper *glHelper;
	int elapsed;

	QMap<int, int> memoryMap;
};

#endif