#include "lemviewer.h"
#include <QtGui>
#include <QRect>
#include <QImage>
#include <QColor>

#include <iostream>

#include <QMutexLocker>

LemViewer::LemViewer(Emulator *emu, QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	emulator = emu;
	screenAddress = 0;

	setFixedSize(REAL_WIDTH, REAL_HEIGHT);
	setAutoFillBackground(false);


	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			videoBuffer[y][x] = 0;
		}
	}

	elapsed = 0;

}


LemViewer::~LemViewer()
{
}

/*
void LemViewer::drawChar(int x, int y, word_t value) {
int charValue = value & 0x7f;

//qDebug() << QString::number(value);
for (int w = 0; w < CHAR_WIDTH; w++) {
for (int h = 0; h < CHAR_HEIGHT; h++) {
videoBuffer[(x * PIXEL_WIDTH) + w][(y * PIXEL_HEIGHT) + h] = 255;
}
}	
}
*/

void LemViewer::drawLoop() {

	QMutexLocker locker(&mutex);

	word_map memory = emulator->getMemory();

	word_t currentValue;

	char charValue;
	word_t fontChar[2] = {0, 0};

	word_t frontColour, backgroundColour;
	bool blink;

	word_t word, hword;
	word_t pixel;

	if (memory.size() > 0) {
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLUMNS; c++) {

				currentValue = memory[screenAddress + (r * COLUMNS) + c];

				charValue = currentValue & 0x7f;
				frontColour = getColour((currentValue >> 12) & 0xf);
				backgroundColour = getColour((currentValue >> 8) & 0xf);
				blink = (((currentValue >> 7) & 1) == 1);

				fontChar[0] = defaultFont[charValue * 2];
				fontChar[1] = defaultFont[charValue * 2 + 1];

				for (int w = 0; w < CHAR_WIDTH; w++) {
					word = fontChar[(w >= 2) * 1];
					hword = (word >> (!(w % 2) * 8)) & 0xff;

					for (int h = 0; h < CHAR_HEIGHT; h++) {

						pixel = (hword >> h) & 1;

						if (pixel) {
							videoBuffer[(c * h)][(r * w)] = 255;
						}
					}
				}	
				//std::cout << QString::number(memory[screenAddress + (r * COLUMNS) + c], 16).toStdString() + " ";
				//lemViewer->drawChar(c, r, memory[screenRamAddress + (r * COLUMNS) + r]);
			}

			//std::cout << std::endl;
		}
	}
}

word_t LemViewer::getColour(word_t value) {
	value &= 0xf;

	word_t colour = defaultPalette[value];

	return colour;
}

void LemViewer::setScreenAddress(long ramAddress) {
	screenAddress = ramAddress;
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

	//painter.setBrush(QBrush("#539e47"));

	QImage image(REAL_WIDTH, REAL_HEIGHT, QImage::Format_RGB32);
	QRgb value;

	word_t pixel;

	// Calculate what characters are visible
	drawLoop();

	for (int y = 0; y < REAL_HEIGHT; y++) {
		for (int x = 0; x < REAL_WIDTH; x++) {

			// Data about current pixel
			//	if (x % 2 == 0) {

			//qDebug() << QString::number((int)x / PIXEL_WIDTH) + "x" + QString::number((int)y / PIXEL_HEIGHT);

			/*
			if ((int)x / PIXEL_WIDTH) {
			std::cout << "b";
			}

			if ((int)y / PIXEL_HEIGHT) {
			std::cout << std::endl;
			}
			*/

			pixel = videoBuffer[(int)y / PIXEL_HEIGHT][(int)x / PIXEL_WIDTH];

			//	qDebug() << pixel;

			//}

			//	qDebug() << QString::number(x);

			//if (x % 2 == 0) {
			//qDebug() << QString::number(x);
			//qDebug() << "Adding pixel slice";

			value = qRgb(pixel, 0, 0);

			/*
			qDebug() << QString::number((x * PIXEL_WIDTH)) + "x" + QString::number((y * PIXEL_HEIGHT));
			qDebug() << QString::number((x * PIXEL_WIDTH) + 1) + "x" + QString::number((y * PIXEL_HEIGHT) + 1);
			qDebug() << QString::number((x * PIXEL_WIDTH) + 2) + "x" + QString::number((y * PIXEL_HEIGHT) + 2);
			*/

			image.setPixel(x, y, value);
			//image.setPixel((x * PIXEL_WIDTH) + 1, y, value);
			//image.setPixel((x * PIXEL_WIDTH) + 2, y, value);

			//}

		}
	}


	/*
	uchar *rawData = new uchar[WIDTH * HEIGHT];

	for (int y = 0; y < HEIGHT; y++) {
	for (int x = 0; x < WIDTH; x++) {
	rawData[WIDTH * HEIGHT] = 100;
	//painter.drawRect(x, y, 4, 8);
	}
	}


	QPixmap buffer;

	buffer.convertFromImage(QImage(rawData, WIDTH, HEIGHT));
	*/

	QPixmap buffer;
	buffer.convertFromImage(image);

	painter.drawPixmap(0, 0, buffer);

	//painter.drawRect(0, 0, 4, 8);

	painter.end();

	//delete[] rawData;
}
