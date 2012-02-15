#ifndef SCOLORWIDGET_H
#define SCOLORWIDGET_H

#include <QDockWidget>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "SGlobals.h"

typedef enum{
    eColor_R,
    eColor_G,
    eColor_B,
    eColor_A
}eColor;

class SColorPicker : public QWidget
{
    Q_OBJECT
public:
    SColorPicker(eColor color, QWidget* parent=0, const char* name="SColorPicker");
    ~SColorPicker();

private:
    void setupColor(eColor c);

    QHBoxLayout* mpLayout;
    QLabel* mpIndicator;
    QVBoxLayout* mpColorLayout;
    QLabel* mpColorFrame;
    QSlider* mpColorValue;
};

class SColorWidget : public QDockWidget
{
    Q_OBJECT
public:
    SColorWidget(QWidget* parent=0, const char* name="SColorWidget");
    ~SColorWidget();

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    QLabel* mpColorLabel;
    SColorPicker* mpRedPicker;
    SColorPicker* mpGreenPicker;
    SColorPicker* mpBluePicker;
    SColorPicker* mpAlphaPicker;
};

#endif // SCOLORWIDGET_H
