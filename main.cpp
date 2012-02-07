#include <QtGui/QApplication>
#include "smainwnd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SMainWnd w;
    w.showMaximized();

    return a.exec();
}
