#ifndef SBRUSHPROPERTIESWIDGET_H
#define SBRUSHPROPERTIESWIDGET_H

#include <QDockWidget>
#include <QWidget>
#include <QVBoxLayout>

#include "SDockPalette.h"
#include "customwidgets/STopicTitleLabel.h"
#include "customwidgets/SSlider.h"
#include "customwidgets/SCheckBox.h"
#include "customwidgets/SBrushPreviewWidget.h"
#include "drawing/SDrawingController.h"
#include "drawing/SBrush.h"

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
    void onPressureReactionChanged();
    void onBrushChanged(SBrush* b);
    void onSpacingChanged(int s);
    void onHardnessChanged(int h);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    STopicTitleLabel* mpLineWidthLabel;
    SSlider* mpWidthSlider;
    SCheckBox* mpWidthPressureCB;
    SCheckBox* mpOpacityPressureCB;
    SBrushPreviewWidget* mpBrushPreview;
    STopicTitleLabel* mpSpacingLabel;
    SSlider* mpSpacingSlider;
    SSlider* mpHardnessSlider;
    STopicTitleLabel* mpHardnessLabel;
};

#endif // SBRUSHPROPERTIESWIDGET_H
