#include <QDebug>
#include "smainwnd.h"

SMainWnd::SMainWnd(QWidget *parent):QMainWindow(parent)
  , mpDrawingView(NULL)
  , mpInfos(NULL)
{
    mpDrawingView = new SDrawingView(this);
    setCentralWidget(mpDrawingView);
//    mpInfos = new QLabel(this);
//    mpInfos->setText(tr("Current Point: "));
//    mpInfos->setGeometry(10, 10, 200, 50);
    connect(mpDrawingView, SIGNAL(currentPointChanged(QPointF)), this, SLOT(onCurrentPointChanged(QPointF)));

}

SMainWnd::~SMainWnd()
{
    DELETEPTR(mpInfos);
    DELETEPTR(mpDrawingView);
}

void SMainWnd::onCurrentPointChanged(QPointF p)
{
    qDebug() << "Current Point: " << p.x() << ";" << p.y();
    //mpInfos->setText(tr("Current Point: %0;%1").arg(p.x()).arg(p.y()));
}
