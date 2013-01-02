#ifndef _LEMVIEWER_H
#define _LEMVIEWER_H

#include <QGLWidget>

class LemViewer :
	public QGLWidget
{
public:
	LemViewer(QWidget *parent = 0);
	~LemViewer();

	public slots:
		void animate();

protected:
	void paintEvent(QPaintEvent *event);

private:
	int elapsed;
};

#endif