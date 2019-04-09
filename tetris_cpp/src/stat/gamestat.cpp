#include <QtWidgets>
#include "gamestat.h"
#include "../window/RunParameter.h"

GameStat::GameStat(QWidget *parent) : QWidget(parent)
{
	setFixedSize(WIDGET_W, WIDGET_H);
	ftime(&tb);
	STAT_GAMESTART_TIME = tb.time;
}

void GameStat::paintEvent(QPaintEvent *)
{
	QPainter p(this);
	p.setBrush(Qt::black);
	p.drawRect(rect());

	p.setPen(Qt::white);
	int marginTop = 20;
	int offset = 35;
	QRectF textRect(0, marginTop, WIDGET_W / 2, 18);

	p.drawText(textRect, Qt::AlignHCenter|Qt::AlignVCenter, QString("成功盘数: %1").arg(STAT_SUCCESSTIMES));

	textRect.moveBottom((marginTop += offset));
	p.drawText(textRect, Qt::AlignHCenter|Qt::AlignVCenter, QString("总盘数: %1").arg(STAT_GAMETIMES));

	textRect.moveBottom((marginTop += offset));
	ftime(&tb);
	int gameDuration = tb.time - STAT_GAMESTART_TIME;
	int mins = gameDuration / 60;
	int secend = gameDuration % 60;
	p.drawText(textRect, Qt::AlignHCenter|Qt::AlignVCenter, QString("游戏时长:  %1分 %2秒").arg(mins).arg(secend));
}
