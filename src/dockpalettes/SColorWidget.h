#ifndef SCOLORWIDGET_H
#define SCOLORWIDGET_H

#include <QDockWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "SDockPalette.h"
#include "STopicTitleLabel.h"
#include "SSlider.h"

typedef enum{
    eColor_R,
    eColor_G,
    eColor_B,
    eColor_A
}eColor;

class SColorThumbnail : public QLabel
{
    Q_OBJECT
public:
    SColorThumbnail(QWidget* parent=0, const char* name="SColorThumbnail");
    ~SColorThumbnail();
    void setColor(const QColor& color);
    QColor color();

private:
    QColor mColor;
};

class SColorPreview : public QWidget
{
    Q_OBJECT
public:
    SColorPreview(QWidget* parent=0, const char* name="SColorPreview");
    ~SColorPreview();
    void setColor(const QColor& color);

protected:
    void resizeEvent(QResizeEvent* ev);

private:
    SColorThumbnail* mpBackColor;
    SColorThumbnail* mpForeColor;
};

class SColorPicker : public QWidget
{
    Q_OBJECT
public:
    SColorPicker(eColor color, QWidget* parent=0, const char* name="SColorPicker");
    ~SColorPicker();
    void setValue(int value);
    int value();

signals:
    void colorComponentChanged();

private slots:
    void onValueChanged(int value);

private:
    void setupColor(eColor c);

    QHBoxLayout* mpLayout;
    QLabel* mpIndicator;
    QVBoxLayout* mpColorLayout;
    QLabel* mpColorFrame;
    SSlider* mpColorValue;
    bool mSettingValue;
};

class SColorWidget : public SDockPalette
{
    Q_OBJECT
public:
    SColorWidget(QWidget* parent=0, const char* name="SColorWidget");
    ~SColorWidget();
    void setColor(const QColor& color);
    QColor color();

signals:
    void colorChanged(const QColor& color);

private slots:
    void onColorComponentChanged();

private:
    QWidget* mpContainer;
    QVBoxLayout* mpContainerLayout;
    STopicTitleLabel* mpColorLabel;
    SColorPicker* mpRedPicker;
    SColorPicker* mpGreenPicker;
    SColorPicker* mpBluePicker;
    SColorPicker* mpAlphaPicker;
    SColorPreview* mpColorPreview;
    QColor mColor;
};

#endif // SCOLORWIDGET_H
