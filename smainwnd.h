#ifndef SMAINWND_H
#define SMAINWND_H

#include <QMainWindow>
#include <QLabel>
#include <QPointF>

#include "SGlobals.h"
#include "SDrawingView.h"

class SMainWnd : public QMainWindow
{
    Q_OBJECT

public:
    SMainWnd(QWidget *parent = 0);
    ~SMainWnd();

private slots:
    void onCurrentPointChanged(QPointF p);

private:
    SDrawingView* mpDrawingView;
    QLabel* mpInfos;
};

#endif // SMAINWND_H
