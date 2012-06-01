#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):SDockPalette(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
{
    Q_UNUSED(name);
    SETUP_STYLESHEET
    setWindowTitle(tr("Brush Properties"));
    mpContainer = new QWidget(this);
    mpContainer->setObjectName(ON_PALETTE_CONTAINER);
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    mpLineWidthLabel = new STopicTitleLabel(tr("Line Width"), mpContainer);
    mpContainerLayout->addWidget(mpLineWidthLabel, 0);
    mpWidthSlider = new SSlider(Qt::Horizontal, mpContainer);
    mpWidthSlider->setMinimum(1);
    mpWidthSlider->setValue(3);
    mpWidthSlider->setMaximum(100);
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
