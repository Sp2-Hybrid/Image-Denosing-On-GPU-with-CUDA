#include "one.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	one w;
	w.show();

	return a.exec();
}