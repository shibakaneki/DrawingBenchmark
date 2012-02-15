#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):QDockWidget(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
{
    setObjectName(name);
    setWindowTitle(tr("Brush Properties"));
    mpContainer = new QWidget(this);
    mpContainer->setStyleSheet("border-radius:5px; border:2px solid #B6CFD6; background:white; margin-left: 5px; margin-bottom:5px;");
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    mpLineWidthLabel = new QLabel(tr("Line Width"), mpContainer);
    mpLineWidthLabel->setStyleSheet("border:none; font-size:16px; margin-bottom:2px; color:#7CA7B3");
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
