#ifndef SSETTINGSWIDGET_H
#define SSETTINGSWIDGET_H

#include <QDockWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
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
    void onClearCoefficients();
    void onAddCoefficients(QPointF p0, QPointF p1, QPointF c0, QPointF c1);

signals:
    void smoothnessChanged(int smoothness);
    void pointSelected(QPointF p0, QPointF p1, QPointF c0, QPointF c1);

private slots:
    void onValueChanged(int val);
    void onPointSelected(QTreeWidgetItem* crnt, QTreeWidgetItem* prev);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    QLabel* mpSmoothnessLabel;
    QSlider* mpSmoothness;
    QLabel* mpInfosLabel;
    QLabel* mpXCoord;
    QLabel* mpYCoord;
    QLabel* mpCoefficientsLabel;
    QTreeWidget* mpTable;
    QTreeWidgetItem* mpTableItem;
};

#endif // SSETTINGSWIDGET_H
