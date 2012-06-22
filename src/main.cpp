#include <QApplication>
#include <QCleanlooksStyle>

#include "workspace/SMainWnd.h"
#include "maths/SGeometryHelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SGeometryHelper::init();
#ifdef Q_WS_X11
    a.setGraphicsSystem("raster");
#endif
    QCleanlooksStyle s;
    //a.setStyle(&s);
    SMainWnd w;
    w.showMaximized();

    return a.exec();
}
