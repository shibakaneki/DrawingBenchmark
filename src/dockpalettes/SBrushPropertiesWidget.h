#ifndef SBRUSHPROPERTIESWIDGET_H
#define SBRUSHPROPERTIESWIDGET_H

#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>

#include "SDockPalette.h"
#include "customwidgets/STopicTitleLabel.h"
#include "customwidgets/SSlider.h"

class SBrushPropertiesWidget : public SDockPalette
{
    Q_OBJECT
public:
    SBrushPropertiesWidget(QWidget* parent=0, const char* name="SBrushPropertiesWidget");
    virtual ~SBrushPropertiesWidget();

signals:
    void lineWidthChanged(int w);

private slots:
    void onLineWidthChanged(int w);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    STopicTitleLabel* mpLineWidthLabel;
    SSlider* mpWidthSlider;
};

#endif // SBRUSHPROPERTIESWIDGET_H
