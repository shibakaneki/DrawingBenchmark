#include "SColorWidget.h"

SColorWidget::SColorWidget(QWidget *parent, const char *name):QDockWidget( parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpColorLabel(NULL)
  , mpRedPicker(NULL)
  , mpGreenPicker(NULL)
  , mpBluePicker(NULL)
  , mpAlphaPicker(NULL)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setWindowTitle(tr("Colors"));

    mpContainer = new QWidget(this);
    mpContainer->setObjectName("SPaletteContainer");
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    mpColorLabel = new QLabel(tr("Line Color"), mpContainer);
    mpColorLabel->setObjectName("SPaletteTopicTitleLabel");
    mpRedPicker = new SColorPicker(eColor_R, mpContainer);
    mpRedPicker->setStyleSheet("border:none; margin-top:0px;");
    mpGreenPicker = new SColorPicker(eColor_G, mpContainer);
    mpGreenPicker->setStyleSheet("border:none; margin-top:0px;");
    mpBluePicker = new SColorPicker(eColor_B, mpContainer);
    mpBluePicker->setStyleSheet("border:none; margin-top:0px;");
    mpAlphaPicker = new SColorPicker(eColor_A, mpContainer);
    mpAlphaPicker->setStyleSheet("border:none; margin-top:0px;");

    mpContainerLayout->addWidget(mpColorLabel, 0);
    mpContainerLayout->addWidget(mpRedPicker, 0);
    mpContainerLayout->addWidget(mpGreenPicker, 0);
    mpContainerLayout->addWidget(mpBluePicker, 0);
    mpContainerLayout->addWidget(mpAlphaPicker, 0);
    mpContainerLayout->addStretch(1);
}

SColorWidget::~SColorWidget()
{
    DELETEPTR(mpAlphaPicker);
    DELETEPTR(mpBluePicker);
    DELETEPTR(mpGreenPicker);
    DELETEPTR(mpRedPicker);
    DELETEPTR(mpColorLabel);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

// ----------------------------------------------------------------------------------------
SColorPicker::SColorPicker(eColor color, QWidget *parent, const char *name):QWidget(parent)
  , mpLayout(NULL)
  , mpIndicator(NULL)
  , mpColorValue(NULL)
{
    setObjectName(name);
    mpLayout = new QHBoxLayout();
    setLayout(mpLayout);

    mpIndicator = new QLabel(this);
    mpLayout->addWidget(mpIndicator, 0);

    mpColorValue = new QSlider(this);
    mpColorValue->setOrientation(Qt::Horizontal);
    mpColorValue->setMinimum(0);
    mpColorValue->setMaximum(255);
    mpLayout->addWidget(mpColorValue, 1);

    setupColor(color);
}

SColorPicker::~SColorPicker()
{
    DELETEPTR(mpColorValue);
    DELETEPTR(mpIndicator);
    DELETEPTR(mpLayout);
}

void SColorPicker::setupColor(eColor c)
{
    // TODO: customize the sliders and add a rectangle showing the current color related to all the color choices
    switch(c){
    case eColor_R:
        mpIndicator->setText("R");
        mpColorValue->setStyleSheet("background:qlineargradient(x1:0, x2:1,stop:0 black, stop:1 red);");
        break;
    case eColor_G:
        mpIndicator->setText("G");
        mpColorValue->setStyleSheet("background:qlineargradient(x1:0, x2:1,stop:0 black, stop:1 green);");
        break;
    case eColor_B:
        mpIndicator->setText("B");
        mpColorValue->setStyleSheet("background:qlineargradient(x1:0, x2:1,stop:0 black, stop:1 blue);");
        break;
    case eColor_A:
        mpIndicator->setText("A");
        mpColorValue->setStyleSheet("background:qlineargradient(x1:0, x2:1,stop:0 black, stop:1 white);");
        break;
    }
}
