#include <QtWidgets>
#include "mainwindow.h"
#include "../stat/gamestat.h"
#include "../gamepool/gamepool.h"
#include "../gamepool/PoolConstant.h"

MainWindow::MainWindow()
{
	setupUi(this);
	gamestat = new GameStat(statWidget);
	gamepool = new GamePool(poolWidget);
	gamepool->setGameStat(gamestat);
	gamepool->setFocusPolicy(Qt::StrongFocus);

	createAction();
}

void MainWindow::createAction()
{
	connect(startBtn, SIGNAL(clicked()), gamepool, SLOT(startGame()));
	connect(exitBtn, SIGNAL(clicked()), this, SLOT(close()));
}
