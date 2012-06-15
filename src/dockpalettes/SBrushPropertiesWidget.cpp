#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):SDockPalette(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
  , mpWidthPressureCB(NULL)
  , mpOpacityPressureCB(NULL)
  , mpBrushPreview(NULL)
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
    mpWidthSlider->setValue(SDrawingController::drawingController()->currentBrush()->width());
    mpWidthSlider->setMaximum(100);
    mpContainerLayout->addWidget(mpWidthSlider, 0);

    mpWidthPressureCB = new SCheckBox(this);
    mpWidthPressureCB->setText(tr("size related to pressure"));
    mpContainerLayout->addWidget(mpWidthPressureCB, 0);

    mpOpacityPressureCB = new SCheckBox(this);
    mpOpacityPressureCB->setText(tr("opacity related to pressure"));
	mpContainerLayout->addWidget(mpOpacityPressureCB, 0);

	mpBrushPreview = new SBrushPreviewWidget(this);
	mpContainerLayout->addWidget(mpBrushPreview, 0);

	mpContainerLayout->addStretch(1);

    connect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
    connect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
}

SBrushPropertiesWidget::~SBrushPropertiesWidget()
{
	DELETEPTR(mpBrushPreview);
	DELETEPTR(mpOpacityPressureCB);
	DELETEPTR(mpWidthPressureCB);
    DELETEPTR(mpWidthSlider);
    DELETEPTR(mpLineWidthLabel);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

void SBrushPropertiesWidget::onLineWidthChanged(int w)
{
    emit lineWidthChanged(w);
}

void SBrushPropertiesWidget::onPressureReactionChanged(){
	SDrawingController::drawingController()->currentBrush()->setWidthPressureSensitive(mpWidthPressureCB->isChecked());
	SDrawingController::drawingController()->currentBrush()->setOpacityPressureSensitive(mpOpacityPressureCB->isChecked());
}

void SBrushPropertiesWidget::onBrushChanged(SBrush* b){
	if(NULL != b){
		disconnect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
		disconnect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
		mpWidthPressureCB->setChecked(b->isWidthPressureSensitive());
		mpOpacityPressureCB->setChecked(b->isOpacityPressureSensitive());
		connect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
		connect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
	}
}

