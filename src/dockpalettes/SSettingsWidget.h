#ifndef SSETTINGSWIDGET_H
#define SSETTINGSWIDGET_H

#include <QDockWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "SDockPalette.h"
#include "STopicTitleLabel.h"
#include "SSlider.h"

class SSettingsWidget : public SDockPalette
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

protected:
    void enterEvent(QEvent* ev);
    void leaveEvent(QEvent* ev);

private slots:
    void onValueChanged(int val);
    void onPointSelected(QTreeWidgetItem* crnt, QTreeWidgetItem* prev);

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    STopicTitleLabel* mpSmoothnessLabel;
    SSlider* mpSmoothness;
    STopicTitleLabel* mpInfosLabel;
    QLabel* mpXCoord;
    QLabel* mpYCoord;
    STopicTitleLabel* mpCoefficientsLabel;
    QTreeWidget* mpTable;
    QTreeWidgetItem* mpTableItem;
};

#endif // SSETTINGSWIDGET_H
