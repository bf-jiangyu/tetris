#include "block.h"
#include "../PoolConstant.h"
#include "../../window/RunParameter.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
#include <cstdlib>

Block::Block(int shape)
{/*{{{*/
	if (shape < 0) return;
	this->shape = shape;
	switch (shape) {
		case 0:
			{
				this->x = 2;
				this->y = 1;
				this->img = "T";
				this->initT();
				break;
			}
		case 1:
			{
				this->x = 2.5;
				this->y = 1.5;
				this->img = "L";
				this->initL();
				break;
			}
		case 2:
			{
				this->x = 1.5;
				this->y = 1.5;
				this->img = "J";
				this->initJ();
				break;
			}
		case 3:
			{
				this->x = 2;
				this->y = 2;
				this->img = "Z";
				this->initZ();
				break;
			}
		case 4:
			{
				this->x = 2;
				this->y = 2;
				this->img = "S";
				this->initS();
				break;
			}
		case 5:
			{
				this->x = 1.5;
				this->y = 1.5;
				this->img = "I";
				this->initI();
				break;
			}
		case 6:
			{
				this->x = 1.5;
				this->y = 1;
				this->img = "O";
				this->initO();
				break;
			}
	}
}/*}}}*/

void Block::draw(QPainter &p)
{
	if (gameover) p.setOpacity(0.5);
	QPixmap cell;
	cell.load(IMG_PATH + "" + img + ".png");
	for (auto &point : cells) {
		p.drawPixmap((*point) * CELL_WIDTH, (*(point + 1)) * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, cell);
	}
}

void Block::turnD()
{
	++status;
	initShape();
	if (!checkPool()) {
		--status;
		initShape();
	}
}

void Block::turnS()
{
	--status;
	initShape();
	if (!checkPool()) {
		++status;
		initShape();
	}
}

bool Block::moveLeft()
{
	--x;
	initShape();
	if (!checkPool()) {
		++x;
		initShape();
		return false;
	}
	return true;
}
void Block::moveLeft3()
{
	int t = 0;
	while (t++ < 3 && moveLeft());
}
void Block::moveLeftEnd()
{
	while (moveLeft());
}

bool Block::moveRight()
{
	++x;
	initShape();
	if (!checkPool()) {
		--x;
		initShape();
		return false;
	}
	return true;
}
void Block::moveRight3()
{
	int t = 0;
	while (t++ < 3 && moveRight());
}
void Block::moveRightEnd()
{
	while (moveRight());
}

void Block::moveTop()
{
	--y;
	initShape();
}

bool Block::moveDown()
{
	++y;
	initShape();
	if (!checkPool()) {
		if (y) --y;
		initShape();
		return false;
	}
	return true;
}

void Block::moveBottom()
{
	while (moveDown());
}

void Block::putBlock()
{
	for (auto &point : cells)
	{
		poolMap[int(*(point + 1))][int(*point)] = img;
	}
}

void Block::clearBlock()
{
	for (auto &point : cells)
	{
		poolMap[int(*(point + 1))][int(*point)] = "";
	}
}

void Block::setShow()
{
	this->x = 5;
	this->y = 1;
	this->status = 0;
	initShape();
}

bool Block::checkPool()
{
	for (auto &point : cells)
		if ((*point) < 0 || (*point) >= POOL_W) return false;
		else if (*(point + 1) < 0 || *(point + 1) >= POOL_H) return false;
	for (int i = 0; i < POOL_H; ++i)
		for (int j = 0; j < POOL_W; ++j)
			if (!poolMap[i][j].isEmpty()) {
				for (auto &point : cells)
					if ((*point) == j && *(point + 1) == i) {
						return false;
					}
			}
	return true;
}

void Block::clearRepeatCell()
{
	for (auto &dpoint : cells)
		poolMap[(int)(*(dpoint + 1))][(int)(*dpoint)] = "";
}

Block Block::clone()
{
	return Block(shape);
}

void Block::initShape()
{
	int maxStatus = 3;
	if (shape > 2) maxStatus = 1;

	if (shape == 6)
		status = 0;
	else {
		if (status > maxStatus) status = 0;
		else if (status < 0) status = maxStatus;
	}

	switch (shape) {
		case 0: initT(); break;
		case 1: initL(); break;
		case 2: initJ(); break;
		case 3: initZ(); break;
		case 4: initS(); break;
		case 5: initI(); break;
		case 6: initO(); break;
	}
}

void Block::initT()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x - 1;
		cells[1][1] = y;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	} else if (status == 1) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x - 1;
		cells[2][1] = y;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	} else if (status == 2) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x - 1;
		cells[2][1] = y;
		cells[3][0] = x + 1;
		cells[3][1] = y;
	} else if (status == 3) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	}
}

void Block::initL()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x - 1;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y - 1;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	} else if (status == 1) {
		cells[1][0] = x + 1;
		cells[1][1] = y - 1;
		cells[2][0] = x - 1;
		cells[2][1] = y;
		cells[3][0] = x + 1;
		cells[3][1] = y;
	} else if (status == 2) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y + 1;
		cells[3][0] = x + 1;
		cells[3][1] = y + 1;
	} else if (status == 3) {
		cells[1][0] = x - 1;
		cells[1][1] = y;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x - 1;
		cells[3][1] = y + 1;
	}
}

void Block::initJ()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x + 1;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y - 1;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	} else if (status == 1) {
		cells[1][0] = x + 1;
		cells[1][1] = y + 1;
		cells[2][0] = x - 1;
		cells[2][1] = y;
		cells[3][0] = x + 1;
		cells[3][1] = y;
	} else if (status == 2) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y + 1;
		cells[3][0] = x - 1;
		cells[3][1] = y + 1;
	} else if (status == 3) {
		cells[1][0] = x - 1;
		cells[1][1] = y;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x - 1;
		cells[3][1] = y - 1;
	}
}

void Block::initZ()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x - 1;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y - 1;
		cells[3][0] = x + 1;
		cells[3][1] = y;
	} else if (status == 1) {
		cells[1][0] = x + 1;
		cells[1][1] = y - 1;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x;
		cells[3][1] = y + 1;
	}
}

void Block::initS()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x + 1;
		cells[1][1] = y - 1;
		cells[2][0] = x;
		cells[2][1] = y - 1;
		cells[3][0] = x - 1;
		cells[3][1] = y;
	} else if (status == 1) {
		cells[1][0] = x;
		cells[1][1] = y - 1;
		cells[2][0] = x + 1;
		cells[2][1] = y;
		cells[3][0] = x + 1;
		cells[3][1] = y + 1;
	}
}

void Block::initI()
{
	cells[0][0] = x;
	cells[0][1] = y;
	if (status == 0) {
		cells[1][0] = x + 1;
		cells[1][1] = y;
		cells[2][0] = x + 2;
		cells[2][1] = y;
		cells[3][0] = x - 1;
		cells[3][1] = y;
	} else if (status == 1) {
		cells[0][0] = x + 1;
		cells[1][0] = x + 1;
		cells[1][1] = y - 1;
		cells[2][0] = x + 1;
		cells[2][1] = y + 1;
		cells[3][0] = x + 1;
		cells[3][1] = y + 2;
	}
}

void Block::initO()
{
	cells[0][0] = x;
	cells[0][1] = y;
	cells[1][0] = x + 1;
	cells[1][1] = y;
	cells[2][0] = x + 1;
	cells[2][1] = y + 1;
	cells[3][0] = x;
	cells[3][1] = y + 1;
}
