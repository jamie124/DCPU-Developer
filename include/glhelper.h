#ifndef _GLHELPER_H
#define _GLHELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>

#include "emulator.h"

class QPainter;
class QPaintEvent;

static int BLOCK_HEIGHT = 20;
static int BLOCK_WIDTH = 36;
static int WIDTH_OFFSET = 25;

class GLHelper
{
public:
	GLHelper(void);
	~GLHelper(void);

	void setWindowSize(int width, int hight);
	long getTotalRows();
	int getRowsPerWindow();

	void setRowOffset(int value);

	void paint(QPainter *painter, word_map &memoryMap, QPaintEvent *event, int elapsed);

private:
	QBrush background;
	QFont textFont;
	QPen textPen;

	int viewerWidth, viewerHeight;
	int rows, blocksPerRow;
	int rowOffset;							// Number of rows to scroll 
	int highlightedX, highlightedY;			// TODO: Will be needed for highlighting a block of data.
};

#endif
