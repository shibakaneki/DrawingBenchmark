#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):SDockPalette(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
  , mpWidthPressureCB(NULL)
  , mpOpacityPressureCB(NULL)
  , mpBrushPreview(NULL)
  , mpCubicLevelLabel(NULL)
  , mpCubicLevelSlider(NULL)
  , mpCubicSmoothnessLabel(NULL)
  , mpCubicSmoothnessSlider(NULL)
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

	mpCubicLevelLabel = new STopicTitleLabel(tr("Cubic Level"), mpContainer);
	mpContainerLayout->addWidget(mpCubicLevelLabel, 0);
	mpCubicLevelSlider = new SSlider(Qt::Horizontal, mpContainer);
	mpCubicLevelSlider->setMinimum(2);
	mpCubicLevelSlider->setMaximum(10);
	mpCubicLevelSlider->setValue(SDrawingController::drawingController()->interpolationLevel());
	mpContainerLayout->addWidget(mpCubicLevelSlider, 0);

	mpCubicSmoothnessLabel = new STopicTitleLabel(tr("Smoothness"), mpContainer);
	mpContainerLayout->addWidget(mpCubicSmoothnessLabel, 0);
	mpCubicSmoothnessSlider = new SSlider(Qt::Horizontal, mpContainer);
	mpCubicSmoothnessSlider->setMinimum(2);
	mpCubicSmoothnessSlider->setMaximum(30);
	mpCubicSmoothnessSlider->setValue(SDrawingController::drawingController()->interpolationStep());
	mpContainerLayout->addWidget(mpCubicSmoothnessSlider, 0);

	mpContainerLayout->addStretch(1);

    connect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
    connect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
    connect(mpCubicLevelSlider, SIGNAL(valueChanged(int)), this, SLOT(onCubicLevelChanged(int)));
    connect(mpCubicSmoothnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onCubicSmoothnessChanged(int)));
}

SBrushPropertiesWidget::~SBrushPropertiesWidget()
{
	DELETEPTR(mpCubicSmoothnessSlider);
	DELETEPTR(mpCubicSmoothnessLabel);
	DELETEPTR(mpCubicLevelSlider);
	DELETEPTR(mpCubicLevelLabel);
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
	mpWidthSlider->setToolTip(QString("%0").arg(w));
    emit lineWidthChanged(w);
}

void SBrushPropertiesWidget::onPressureReactionChanged(){
	disconnect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
	SDrawingController::drawingController()->setWidthPressureSensitive(mpWidthPressureCB->isChecked());
	SDrawingController::drawingController()->setOpacityPressureSensitive(mpOpacityPressureCB->isChecked());
	connect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
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

void SBrushPropertiesWidget::onCubicLevelChanged(int v){
	SDrawingController::drawingController()->setInterpolationLevel(v);
	mpCubicLevelSlider->setToolTip(QString("%0").arg(v));
}

void SBrushPropertiesWidget::onCubicSmoothnessChanged(int v){
	SDrawingController::drawingController()->setInterpolationStep(v);
	mpCubicSmoothnessSlider->setToolTip(QString("%0").arg(v));
}
