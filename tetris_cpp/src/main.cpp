#include <QApplication>
#include <QDesktopWidget>
#include "window/mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow *win = new MainWindow;
	win->show();
	win->move((QApplication::desktop()->width() - win->width()) / 2 - 300
			, (QApplication::desktop()->height() - win->height()) / 2 - 40);
	return app.exec();
}
