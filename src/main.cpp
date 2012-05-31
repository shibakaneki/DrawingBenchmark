#include <QApplication>
#include <QDebug>

#include "workspace/SMainWnd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_WS_X11
    qDebug() << "Setting the graphic system to Raster";
    a.setGraphicsSystem("raster");
#endif
    SMainWnd w;
    w.showMaximized();

    return a.exec();
}
