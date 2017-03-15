#include "spatial_pointer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpatialPointer w;
    w.show();

    return a.exec();
}
