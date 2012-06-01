#include <QApplication>
#include <QCleanlooksStyle>

#include "workspace/SMainWnd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_WS_X11
    a.setGraphicsSystem("raster");
#endif
    QCleanlooksStyle s;
    a.setStyle(&s);
    SMainWnd w;
    w.showMaximized();

    return a.exec();
}
