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

	initialised = false;

	elapsed = 0;

}


LemViewer::~LemViewer()
{
}

void LemViewer::queueChar(int c, int r) {
	if (c < COLUMNS && r < ROWS) {
		cellQueue[c][r] = true;
	}
}

void LemViewer::drawScreen() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			videoBuffer[x][y] = 0;
		}
	}


	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLUMNS; c++) {
			queueChar(c, r);
		}
	}

	initialised = true;
}

void LemViewer::drawChar(int c, int r) {
	word_t currentValue;

	char charValue;
	word_t fontChar[2] = {0, 0};

	word_t frontColour, backgroundColour;
	bool blink;

	word_t word, hword;
	word_t pixel;

	currentValue = memory[screenAddress + (r * COLUMNS) + c];

	if (currentValue > 0) {

		charValue = currentValue & 0x7f;
		frontColour = getColour((currentValue >> 12) & 0xf);
		backgroundColour = getColour((currentValue >> 8) & 0xf);
		blink = (((currentValue >> 7) & 1) == 1);

		fontChar[0] = defaultFont[charValue * 2];
		fontChar[1] = defaultFont[charValue * 2 + 1];

		//qDebug() << charValue;

		for (int w = 0; w < CHAR_WIDTH; w++) {
			word = fontChar[(w >= 2) * 1];
			hword = (word >> (!(w % 2) * 8)) & 0xff;

			for (int h = 0; h < CHAR_HEIGHT; h++) {

				pixel = (hword >> h) & 1;

				if (pixel) {
					//qDebug() << QString::number((c * w) + w) + ":" + QString::number((r * h) + h);
					//qDebug() << QString::number((c * CHAR_WIDTH) + w) + ":" + QString::number((r * h) + h);

					videoBuffer[(c * CHAR_WIDTH) + w][(r * CHAR_HEIGHT) + h] = 255;
				} 
			}
		}	

		//std::cout << std::endl;
	}
}


void LemViewer::drawLoop() {

	// Shouldn't be set every loop, move.
	memory = emulator->getMemory();

	if (memory.size() > 0) {
		for (int r = 0; r < ROWS; r++) {
			for (int c = 0; c < COLUMNS; c++) {

				if (cellQueue[c][r]) {

					drawChar(c, r);

					cellQueue[c][r] = false;

				}

				//std::cout << QString::number(memory[screenAddress + (r * COLUMNS) + c], 16).toStdString() + " ";
				//lemViewer->drawChar(c, r, memory[screenRamAddress + (r * COLUMNS) + r]);
			}

			//std::cout << std::endl;
		}
	}
}

void LemViewer::updateChar(word_t key) {
	word_t row = (int)floor((double)key / COLUMNS);
	word_t column = key % COLUMNS;

	queueChar(column, row);
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

	if (initialised) {

		drawLoop();


		/*
		for (int y = 0; y < REAL_HEIGHT; y++) {

		for (int c = 0; c < COLUMNS; c++) {


		}
		}
		*/



		for (int y = 0; y < REAL_HEIGHT; y++) {
			for (int x = 0; x < REAL_WIDTH; x++) {

				// Data about current pixel
				//	if (x % 2 == 0) {

				//qDebug() << QString::number((int)x / PIXEL_WIDTH) + "x" + QString::number((int)y / PIXEL_HEIGHT);


				pixel = videoBuffer[(int)x / PIXEL_WIDTH][(int)y / PIXEL_HEIGHT];

				//	qDebug() << pixel;

				//}

				//	qDebug() << QString::number(x);

				//if (x % 2 == 0) {
				//qDebug() << QString::number(x);
				//qDebug() << "Adding pixel slice";

				value = qRgb(pixel, 0, 0);


				//qDebug() << QString::number((x * PIXEL_WIDTH)) + "x" + QString::number((y * PIXEL_HEIGHT));
				//qDebug() << QString::number((x * PIXEL_WIDTH) + 1) + "x" + QString::number((y * PIXEL_HEIGHT) + 1);
				//qDebug() << QString::number((x * PIXEL_WIDTH) + 2) + "x" + QString::number((y * PIXEL_HEIGHT) + 2);


				image.setPixel(x, y, value);
				//image.setPixel((x * PIXEL_WIDTH) + 1, y, value);
				//image.setPixel((x * PIXEL_WIDTH) + 2, y, value);

				//}

			}
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
