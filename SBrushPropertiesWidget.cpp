#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):QDockWidget(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setWindowTitle(tr("Brush Properties"));
    mpContainer = new QWidget(this);
    mpContainer->setObjectName("SPaletteContainer");
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    mpLineWidthLabel = new QLabel(tr("Line Width"), mpContainer);
    mpLineWidthLabel->setObjectName("SPaletteTopicTitleLabel");
    mpContainerLayout->addWidget(mpLineWidthLabel, 0);
    mpWidthSlider = new QSlider(mpContainer);
    mpWidthSlider->setOrientation(Qt::Horizontal);
    mpWidthSlider->setMinimum(1);
    mpWidthSlider->setValue(3);
    mpWidthSlider->setMaximum(100);
    mpWidthSlider->setStyleSheet("border:none; margin-top:0px;");
    mpContainerLayout->addWidget(mpWidthSlider, 0);
    mpContainerLayout->addStretch(1);


    connect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
}

SBrushPropertiesWidget::~SBrushPropertiesWidget()
{
    DELETEPTR(mpWidthSlider);
    DELETEPTR(mpLineWidthLabel);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

void SBrushPropertiesWidget::onLineWidthChanged(int w)
{
    emit lineWidthChanged(w);
}
