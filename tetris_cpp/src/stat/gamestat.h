#ifndef GAMESTAT_H
#define GAMESTAT_H

#include <sys/timeb.h>
#include <QWidget>

class GameStat : public QWidget
{
	Q_OBJECT
public:
	GameStat(QWidget *parent = 0);

protected:
	void paintEvent(QPaintEvent *);

private:
	int WIDGET_W = 400;
	int WIDGET_H = 300;

	struct timeb tb;
};
#endif
