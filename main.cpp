#include <QApplication>
#include "QtSE.h"

int main( int argc , char *argv[] )
{
	QApplication a( argc , argv );
    QtSE w;
    w.show();

    return a.exec();
}
