#include "SBrushPropertiesWidget.h"

SBrushPropertiesWidget::SBrushPropertiesWidget(QWidget *parent, const char *name):SDockPalette(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpLineWidthLabel(NULL)
  , mpWidthSlider(NULL)
  , mpWidthPressureCB(NULL)
  , mpOpacityPressureCB(NULL)
  , mpBrushPreview(NULL)
  , mpSpacingLabel(NULL)
  , mpSpacingSlider(NULL)
  , mpHardnessSlider(NULL)
  , mpHardnessLabel(NULL)
{
    Q_UNUSED(name);
    SETUP_STYLESHEET
    setWindowTitle(tr("Brush Properties"));
    mpContainer = new QWidget(this);
    mpContainer->setObjectName(ON_PALETTE_CONTAINER);
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    // Preview
    mpBrushPreview = new SBrushPreviewWidget(this);
    mpContainerLayout->addWidget(mpBrushPreview, 0);

    // Line width
    mpLineWidthLabel = new STopicTitleLabel(tr("Line Width"), mpContainer);
    mpContainerLayout->addWidget(mpLineWidthLabel, 0);
    mpWidthSlider = new SSlider(Qt::Horizontal, mpContainer);
    mpWidthSlider->setMinimum(1);
    mpWidthSlider->setValue(SDrawingController::drawingController()->currentBrush()->width());
    mpWidthSlider->setMaximum(100);
    mpContainerLayout->addWidget(mpWidthSlider, 0);

    // Pressure sensitive
    mpWidthPressureCB = new SCheckBox(this);
    mpWidthPressureCB->setText(tr("size related to pressure"));
    mpContainerLayout->addWidget(mpWidthPressureCB, 0);

    // Opacity sensitive
    mpOpacityPressureCB = new SCheckBox(this);
    mpOpacityPressureCB->setText(tr("opacity related to pressure"));
	mpContainerLayout->addWidget(mpOpacityPressureCB, 0);

    // Spacing
    mpSpacingLabel = new STopicTitleLabel(tr("Spacing"), mpContainer);
    mpContainerLayout->addWidget(mpSpacingLabel, 0);
    mpSpacingSlider = new SSlider(Qt::Horizontal, mpContainer);
    mpSpacingSlider->setMinimum(1);
    mpSpacingSlider->setMaximum(10);
    mpSpacingSlider->setValue(SDrawingController::drawingController()->spacing());
    mpContainerLayout->addWidget(mpSpacingSlider, 0);

    // Hardness
    mpHardnessLabel = new STopicTitleLabel(tr("Hardness"), mpContainer);
    mpContainerLayout->addWidget(mpHardnessLabel, 0);
    mpHardnessSlider = new SSlider(Qt::Horizontal, mpContainer);
    mpHardnessSlider->setMinimum(1);
    mpHardnessSlider->setMaximum(100);
    mpHardnessSlider->setValue(SDrawingController::drawingController()->hardness());
    mpContainerLayout->addWidget(mpHardnessSlider, 0);

	mpContainerLayout->addStretch(1);

    connect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
    connect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
    connect(SDrawingController::drawingController(), SIGNAL(brushChanged(SBrush*)), this, SLOT(onBrushChanged(SBrush*)));
    connect(mpSpacingSlider, SIGNAL(valueChanged(int)), this, SLOT(onSpacingChanged(int)));
    connect(mpHardnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onHardnessChanged(int)));
}

SBrushPropertiesWidget::~SBrushPropertiesWidget()
{
    DELETEPTR(mpSpacingSlider);
    DELETEPTR(mpSpacingLabel);
	DELETEPTR(mpBrushPreview);
	DELETEPTR(mpOpacityPressureCB);
	DELETEPTR(mpWidthPressureCB);
    DELETEPTR(mpWidthSlider);
    DELETEPTR(mpLineWidthLabel);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

void SBrushPropertiesWidget::onBrushChanged(SBrush* b){
    if(NULL != b){
        disconnect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
        disconnect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
        disconnect(mpSpacingSlider, SIGNAL(valueChanged(int)), this, SLOT(onSpacingChanged(int)));
        disconnect(mpHardnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onHardnessChanged(int)));
        disconnect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
        mpWidthPressureCB->setChecked(b->isWidthPressureSensitive());
        mpOpacityPressureCB->setChecked(b->isOpacityPressureSensitive());
        mpWidthSlider->setValue(b->width());
        mpSpacingSlider->setValue(b->spacing());
        mpHardnessSlider->setValue(b->hardness());
        mpBrushPreview->refresh();
        connect(mpWidthPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
        connect(mpOpacityPressureCB, SIGNAL(stateChanged(int)), this, SLOT(onPressureReactionChanged()));
        connect(mpSpacingSlider, SIGNAL(valueChanged(int)), this, SLOT(onSpacingChanged(int)));
        connect(mpHardnessSlider, SIGNAL(valueChanged(int)), this, SLOT(onHardnessChanged(int)));
        connect(mpWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(onLineWidthChanged(int)));
    }
}

void SBrushPropertiesWidget::onLineWidthChanged(int w)
{
    SDrawingController::drawingController()->setWidth(w);
    mpBrushPreview->refresh();
}

void SBrushPropertiesWidget::onPressureReactionChanged(){
	SDrawingController::drawingController()->setWidthPressureSensitive(mpWidthPressureCB->isChecked());
	SDrawingController::drawingController()->setOpacityPressureSensitive(mpOpacityPressureCB->isChecked());
    mpBrushPreview->refresh();
}

void SBrushPropertiesWidget::onSpacingChanged(int s){
    SDrawingController::drawingController()->setSpacing(s);
    mpBrushPreview->refresh();
}

void SBrushPropertiesWidget::onHardnessChanged(int h){
    SDrawingController::drawingController()->setHardness(h);
    mpBrushPreview->refresh();
}
