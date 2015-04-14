#include "mainwindow.h"
#include <QApplication>

#include "InitESRI.h"
#include "esrigeodatabase.h"

int main(int argc, char *argv[])
{
    InitESRI();
	
    QApplication a(argc, argv);

	ESRIGeometryFactory *f = ESRIGeometryFactory::factory();
	auto pt = f->createGeometry(QCG::GT_Polyline);
	auto c = pt->pointCount();
    MainWindow w;
    w.show();

    return a.exec();
}
