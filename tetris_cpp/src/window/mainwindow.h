#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

class GamePool;
class GameStat;

class MainWindow : public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT
public:
	MainWindow();

private:
	void createAction();

	GamePool *gamepool;
	GameStat *gamestat;
};

#endif
