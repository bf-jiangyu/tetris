#ifndef BLOCK_H
#define BLOCK_H

#include <QString>
#include <QPainter>

class Block
{
public:
	Block(int shape = -1);
	void draw(QPainter &);

	void turnD();
	void turnS();

	bool moveLeft();
	void moveLeft3();
	void moveLeftEnd();
	bool moveRight();
	void moveRight3();
	void moveRightEnd();
	void moveTop();
	bool moveDown();
	void moveBottom();

	bool checkPool();
	void clearRepeatCell();
	void putBlock();
	void clearBlock();
	void setShow();
	Block clone();

private:
	double x;
	double y;
	int shape;
	QString img;
	int status = 0;

	double cells[4][2];

	void initShape();
	void createBlock();

	void initT();
	void initL();
	void initJ();
	void initZ();
	void initS();
	void initI();
	void initO();
};

#endif
