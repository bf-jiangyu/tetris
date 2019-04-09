#ifndef GAMEPOOL_H
#define GAMEPOOL_H
#include <sys/timeb.h>
#include <deque>

#include <QWidget>
#include "block/block.h"
#include "../window/mainwindow.h"

class GamePool : public QWidget
{
	Q_OBJECT
public:
	GamePool(QWidget *parent = 0);
	void setGameStat(GameStat *);

protected:
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
	void paintEvent(QPaintEvent *);
	void timerEvent(QTimerEvent *);

private slots:
	void startGame();

private:
	Block nowBlock;
	Block hintBlock;
	Block hintBlock2;

	std::deque<Block> downDeque;
	std::deque<Block> rollbackDeque;

	int autoRiseTimer = 0;
	int autoDownTimer = 0;

	bool pressDown = false;

	struct timeb tb;
	long startTime = 0;
	long gameoverTime = 0;
	int totalCount = 0;
	int blockSpeed = 0;

	int clickCount = 0;

	int usefulClearLineCount = 0;
	int clearLineCount = 0;

	GameStat *gamestat;

	void createNewBlock();
	void putDownBlock();
	void checkClearLine();
	void rollbackBlock();
	void repairDownDeque();
	void checkGameover();

	void riseLine(bool first = false);

	void initGamePool(QPainter &);
	void drawSpeed(QPainter &);
	void drawGameover(QPainter &);

	QString getSpeedString(int);
	long getSystime();
	void stopTimer(int &);
};

#endif
