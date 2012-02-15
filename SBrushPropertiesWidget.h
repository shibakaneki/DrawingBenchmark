#ifndef SBRUSHPROPERTIESWIDGET_H
#define SBRUSHPROPERTIESWIDGET_H

#include <QDockWidget>
#include <QLabel>
#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>

#include "SGlobals.h"

class SBrushPropertiesWidget : public QDockWidget
{
    Q_OBJECT
public:
    SBrushPropertiesWidget(QWidget* parent=0, const char* name="SBrushPropertiesWidget");
    ~SBrushPropertiesWidget();

signals:
    void lineWidthChanged(int w);

private slots:
    void onLineWidthChanged(int w);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    QLabel* mpLineWidthLabel;
    QSlider* mpWidthSlider;
};

#endif // SBRUSHPROPERTIESWIDGET_H
