#ifndef _MEMORYVIEWER_H
#define _MEMORYVIEWER_H

#include <QGLWidget>

#include "emulator.h"
#include "constants.h"

class GLHelper;
class QPaintEvent;
class QWidget;


class MemoryViewer : public QGLWidget 
{
	Q_OBJECT

public:
	MemoryViewer(GLHelper *glHelper, QWidget *parent = 0);
	~MemoryViewer();

	void setMemoryMap(word_map map);

public slots:
	void animate();

signals:
	void setScrollbarValue(int);

protected:
	void paintEvent(QPaintEvent *event);

private:
	GLHelper *glHelper;
	int elapsed;

	word_map memoryMap;
};

#endif