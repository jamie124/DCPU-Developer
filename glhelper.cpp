#include <QtGui>

#include "include/glhelper.h"

GLHelper::GLHelper()
{

	QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
	gradient.setColorAt(0.0, Qt::white);
	gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

	background = QBrush(QColor(0, 0, 0));

	textPen = QPen(Qt::white);
	textFont.setPixelSize(14);
}

GLHelper::~GLHelper()
{
}

void GLHelper::setWindowSize(int width, int height)
{
	viewerWidth = width;
	viewerHeight = height;

	// Work out how many blocks will fit in each row
	rows = (viewerHeight / BLOCK_HEIGHT) - 1;
	blocksPerRow = ((viewerWidth - WIDTH_OFFSET) / BLOCK_WIDTH) - 1;

	qDebug() << "Height: " << height << "Rows: " << rows;
	qDebug() << "Width: " << width << "Blocks per row: " << blocksPerRow;
}

void GLHelper::paint(QPainter *painter, QMap<int, int> &memoryMap, QPaintEvent *event, int elapsed)
{
	painter->fillRect(event->rect(), background);
	painter->translate(0, 0);

	painter->setPen(textPen);
	painter->setFont(textFont);

	int i = 0;
	int offset = 0;
	QString value = "";

	for (int y = 1; y <= rows; y++) {
		offset += i;

		painter->drawText(0, y * BLOCK_HEIGHT, QString::number(offset).rightJustified(5, '0').toUpper());

		for (int x = 1; x <= blocksPerRow; x++) {

			if (memoryMap.contains(i)) {
				value = QString::number(memoryMap.find(i).value(), 16).rightJustified(4, '0').toUpper();
			} else {
				value = "0000";
			}

			painter->drawText((x * BLOCK_WIDTH ) + WIDTH_OFFSET, y * BLOCK_HEIGHT, value);

			i++;
		}
	}

	// Draw line seperating offset labels
	painter->setPen(QPen(Qt::white, 2));
	painter->drawLine(QPointF(WIDTH_OFFSET * 2, 0), QPointF(WIDTH_OFFSET * 2, viewerHeight));
}