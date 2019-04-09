#include <cstdlib>
#include <QtWidgets>
#include "../stat/gamestat.h"
#include "PoolConstant.h"
#include "../window/RunParameter.h"
#include "gamepool.h"

GamePool::GamePool(QWidget *parent) : QWidget(parent)
{
	setFixedSize(GAMEPOOL_W, GAMEPOOL_HEIGHT);
	setFocusPolicy(Qt::StrongFocus);
	startGame();
}

void GamePool::setGameStat(GameStat *stat)
{
	this->gamestat = stat;
}

void GamePool::startGame()
{
	setFocus();
	for (auto &p : poolMap)
		for (auto &p2 : p)
			p2 = "";

	stopTimer(autoRiseTimer);
	autoRiseTimer = startTimer(AUTO_RISELINE);
	if (OPEN_AUTODOWN) {
		stopTimer(autoDownTimer);
		autoDownTimer = startTimer(AUTO_DOWNBLOCK);
	}

	gameover = false;
	startTime = getSystime();
	gameoverTime = startTime;

	totalCount = 0;
	blockSpeed = 0;
	clickCount = 0;

	usefulClearLineCount = 0;
	clearLineCount = 0;

	downDeque.clear();
	rollbackDeque.clear();

	nowBlock = Block(rand() % 7);
	nowBlock.setShow();
	hintBlock = Block(rand() % 7);
	hintBlock2 = Block(rand() % 7);
	update();
}

void GamePool::putDownBlock()
{
	++totalCount;
	blockSpeed = 100.0 * (totalCount * 1000) / (getSystime() - startTime);
	nowBlock.moveBottom();
	nowBlock.putBlock();
	downDeque.push_back(nowBlock);
	createNewBlock();
	checkClearLine();
}

void GamePool::createNewBlock()
{
	nowBlock = hintBlock;
	nowBlock.setShow();
	hintBlock = hintBlock2;
	if (rollbackDeque.empty())
		hintBlock2 = Block(rand() % 7);
	else {
		hintBlock2 = rollbackDeque.back();
		rollbackDeque.pop_back();
	}
	checkGameover();
}

void GamePool::checkClearLine()
{
	int lineCnt = 0;
	for (int i = 0; i < POOL_H; ++i) {
		int cnt = 0;
		for (int j = 0; j < POOL_W; ++j)
			if (!poolMap[i][j].isEmpty())
				++cnt;
		if (cnt == 12) {
			++lineCnt;
			for (int c = i; c > 0; --c)
				for (int j = 0; j < POOL_W; ++j)
					poolMap[c][j] = poolMap[c - 1][j];
			for (int j = 0; j < POOL_W; ++j)
				poolMap[0][j] = "";
		}
	}
	if (lineCnt) {
		clearLineCount += lineCnt;
		if (lineCnt > 2) usefulClearLineCount += lineCnt;
		downDeque.clear();
		rollbackDeque.clear();
	}
}

void GamePool::rollbackBlock()
{
	if (!downDeque.empty()) {
		--totalCount;
		rollbackDeque.push_back(hintBlock2);
		hintBlock2 = hintBlock;
		hintBlock = nowBlock.clone();
		nowBlock = downDeque.back();
		nowBlock.clearBlock();
		nowBlock.setShow();
		downDeque.pop_back();
	}
}

void GamePool::repairDownDeque()
{
	auto begin = downDeque.begin();
	auto end = downDeque.end();
	while (begin != end) {
		(*begin).moveTop();
		++begin;
	}
}

void GamePool::checkGameover()
{
	if (gameover || !nowBlock.checkPool()) {
		gameover = true;
		gameoverTime = getSystime();

		STAT_TOTALCOUNT += totalCount;
		++STAT_GAMETIMES;
		if (usefulClearLineCount >= 20)
			++STAT_SUCCESSTIMES;

		nowBlock.clearRepeatCell();
		stopTimer(autoRiseTimer);
		stopTimer(autoDownTimer);

		gamestat->update();
	}
}

void GamePool::riseLine(bool first)
{
	if (gameover) return;
	for (int i = 0; i < POOL_W; ++i)
		if (!poolMap[1][i].isEmpty()) {
			gameover = true;
		}
	for (int i = 1; i < POOL_H; ++i)
		for (int j = 0; j < POOL_W; ++j)
			poolMap[i - 1][j] = poolMap[i][j];
	int bottom = POOL_H - 1;
	
	for (int i = 0; i < POOL_W; ++i)
		poolMap[bottom][i] = "";
	for (int i = (first ? 0 : 1); i < POOL_W; i += 2)
		poolMap[bottom][i] = "N";
	repairDownDeque();
	checkGameover();
	update();
}

void GamePool::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_5)
		pressDown = false;
	else
		QWidget::keyReleaseEvent(event);
}
void GamePool::keyPressEvent(QKeyEvent *event)
{
	if (gameover) {
		if (event->key() == Qt::Key_R) {
			startGame();
			update();
		} else {
			QWidget::keyPressEvent(event);
		}
	} else {
		switch (event->key()) {
			case Qt::Key_4:
				{
					if (pressDown) {
						nowBlock.moveBottom();
						while (nowBlock.moveLeft());
						putDownBlock();
					} else {
						nowBlock.moveLeft();
					}
					break;
				}
			case Qt::Key_0: nowBlock.moveLeft3(); break;
			case Qt::Key_7: nowBlock.moveLeftEnd(); break;
			case Qt::Key_6
				:{
					if (pressDown) {
						nowBlock.moveBottom();
						while (nowBlock.moveRight());
						putDownBlock();
					} else {
						nowBlock.moveRight();
					}
					break;
				}
			case Qt::Key_8: nowBlock.moveRight3(); break;
			case Qt::Key_9: nowBlock.moveRightEnd(); break;
			case Qt::Key_5: pressDown = true; nowBlock.moveDown(); break;
			case Qt::Key_Space: putDownBlock(); break;
			case Qt::Key_D: nowBlock.turnS(); break;
			case Qt::Key_F: nowBlock.turnD(); break;

			case Qt::Key_F2: startGame(); break;
			case Qt::Key_R: startGame(); break;
			case Qt::Key_Z: rollbackBlock(); break;
			default:
				QWidget::keyPressEvent(event);
		}
		++clickCount;
		update();
	}
}

void GamePool::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	initGamePool(painter);
}

void GamePool::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == autoDownTimer) {
		if (!gameover)
			if (!nowBlock.moveDown()) {
				putDownBlock();
				update();
			}
	} else if (event->timerId() == autoRiseTimer) {
		if (!gameover) {
			riseLine(true);
			riseLine(false);
		}
	}
}

void GamePool::initGamePool(QPainter &painter)
{
	QPixmap speedPix(HINTPOOL_W, HINTPOOL_H);
	QPixmap hintPix(HINTPOOL_W, HINTPOOL_H);
	QPixmap hintPix2(HINTPOOL_W, HINTPOOL_H);
	QPixmap poolPix(CELLPOOL_W, CELLPOOL_H);

	speedPix.fill(Qt::black);
	hintPix.fill(Qt::black);
	hintPix2.fill(Qt::black);
	poolPix.fill(Qt::black);

	// 方快池已存在块
	QPainter pPool(&poolPix);
	if (gameover) {
		pPool.setOpacity(0.5);
	}
	for (int i = 0; i < POOL_H; ++i)
		for (int j = 0; j < POOL_W; ++j)
			if (!poolMap[i][j].isEmpty()) {
				QPixmap cell;
				cell.load(IMG_PATH + "" + poolMap[i][j] + ".png");
				pPool.drawPixmap(j * CELL_WIDTH, i * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, cell);
			}

	nowBlock.draw(pPool);

	if (gameover)
		drawGameover(pPool);

	QPainter pSpeed(&speedPix);
	drawSpeed(pSpeed);

	QPainter pHint(&hintPix);
	hintBlock.draw(pHint);
	QPainter pHint2(&hintPix2);
	hintBlock2.draw(pHint2);

	painter.drawPixmap(0, 0, speedPix);
	painter.drawPixmap(0, HINTPOOL_T, hintPix);
	painter.drawPixmap(0, HINTPOOL_T * 2, hintPix2);
	painter.drawPixmap(CELLPOOL_L, CELLPOOL_T, poolPix);
}

void GamePool::drawSpeed(QPainter &p)
{
	p.setPen(Qt::white);
	QFont font;
	font.setPointSize(30);
	p.setFont(font);
	QRectF rect(0, 0, HINTPOOL_W, HINTPOOL_H);

	p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, getSpeedString(blockSpeed));
}

void GamePool::drawGameover(QPainter &p)
{
	p.setOpacity(1);
	p.setPen(QColor(Qt::white));
	p.setFont(QFont("黑体", 24, QFont::Bold, false));
	p.drawText(0, CELLPOOL_H / 2 - 10, CELLPOOL_W, 36, Qt::AlignHCenter|Qt::AlignVCenter, "游戏结束");

	p.setFont(QFont("黑体", 12, QFont::Normal, false));

	int marginTop = CELLPOOL_H / 2 + 50;
	int offset = 25;
	QRectF rect(0, marginTop, CELLPOOL_W, 18);

	rect.moveBottom((marginTop += offset));
	QString blockEff = "块效率:	" + getSpeedString(usefulClearLineCount * 300 / totalCount);
	p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, blockEff);

	rect.moveBottom((marginTop += offset));
	QString usefulClear = QString("有用消层: %1").arg(usefulClearLineCount);
	p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, usefulClear);

	rect.moveBottom((marginTop += offset));
	QString totalClear = QString("总消层: %1").arg(clearLineCount);
	p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, totalClear);

	rect.moveBottom((marginTop += offset));
	QString clickSpeed = "按键速度: " + getSpeedString(clickCount * 100000 / (gameoverTime - startTime));
	p.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, clickSpeed);
}

QString GamePool::getSpeedString(int speed)
{
	QString spNum = QString("%1").arg(speed / 100.0);
	if (!speed)
		spNum += ".00";
	else if (speed % 10 == 0)
		spNum += "0";
	return spNum;
}

long GamePool::getSystime()
{
	ftime(&tb);
	return tb.time * 1000 + tb.millitm;
}

void GamePool::stopTimer(int &timerId)
{
	if (timerId) {
		killTimer(timerId);
		timerId = 0;
	}
}
