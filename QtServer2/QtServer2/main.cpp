#include "QtServer2.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtServer2 w;
	w.show();
	return a.exec();
}
