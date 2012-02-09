#ifndef SSETTINGSWIDGET_H
#define SSETTINGSWIDGET_H

#include <QDockWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

#include "SGlobals.h"

class SSettingsWidget : public QDockWidget
{
    Q_OBJECT
public:
    SSettingsWidget(QWidget* parent=0, const char* name="SSettingsWidget");
    ~SSettingsWidget();
    int smoothFactor(){return mpSmoothness->value();}
    void setSmoothFactor(int val){mpSmoothness->setValue(val);}

public slots:
    void onPosChanged(QPointF pos);

signals:
    void smoothnessChanged(int smoothness);

private slots:
    void onValueChanged(int val);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    QLabel* mpSmoothnessLabel;
    QSlider* mpSmoothness;
    QLabel* mpInfosLabel;
    QLabel* mpXCoord;
    QLabel* mpYCoord;
};

#endif // SSETTINGSWIDGET_H
