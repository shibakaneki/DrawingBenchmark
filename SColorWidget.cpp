#include "SColorWidget.h"

SColorWidget::SColorWidget(QWidget *parent, const char *name):SDockPalette( parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpColorLabel(NULL)
  , mpRedPicker(NULL)
  , mpGreenPicker(NULL)
  , mpBluePicker(NULL)
  , mpAlphaPicker(NULL)
{
    Q_UNUSED(name);
    SETUP_STYLESHEET
    setWindowTitle(tr("Colors"));

    mpContainer = new QWidget(this);
    mpContainer->setObjectName("SPaletteContainer");
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    mpColorLabel = new STopicTitleLabel(tr("Line Color"), mpContainer);
    mpRedPicker = new SColorPicker(eColor_R, mpContainer);
    mpGreenPicker = new SColorPicker(eColor_G, mpContainer);
    mpBluePicker = new SColorPicker(eColor_B, mpContainer);
    mpAlphaPicker = new SColorPicker(eColor_A, mpContainer);

    mpContainerLayout->addWidget(mpColorLabel, 0);
    mpContainerLayout->addWidget(mpRedPicker, 0);
    mpContainerLayout->addWidget(mpGreenPicker, 0);
    mpContainerLayout->addWidget(mpBluePicker, 0);
    mpContainerLayout->addWidget(mpAlphaPicker, 0);

    mpColorPreview = new SColorPreview(mpContainer);
    mpContainerLayout->addWidget(mpColorPreview, 0);
    mpContainerLayout->addStretch(1);

    // Init the color
    setColor(QColor(0, 0, 0, 255));

    connect(mpRedPicker, SIGNAL(colorComponentChanged()), this, SLOT(onColorComponentChanged()));
    connect(mpGreenPicker, SIGNAL(colorComponentChanged()), this, SLOT(onColorComponentChanged()));
    connect(mpBluePicker, SIGNAL(colorComponentChanged()), this, SLOT(onColorComponentChanged()));
    connect(mpAlphaPicker, SIGNAL(colorComponentChanged()), this, SLOT(onColorComponentChanged()));
}

SColorWidget::~SColorWidget()
{
    DELETEPTR(mpColorPreview);
    DELETEPTR(mpAlphaPicker);
    DELETEPTR(mpBluePicker);
    DELETEPTR(mpGreenPicker);
    DELETEPTR(mpRedPicker);
    DELETEPTR(mpColorLabel);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

void SColorWidget::setColor(const QColor &color)
{
    mColor = color;
    mpRedPicker->setValue(mColor.red());
    mpGreenPicker->setValue(mColor.green());
    mpBluePicker->setValue(mColor.blue());
    mpAlphaPicker->setValue(mColor.alpha());
    mpColorPreview->setColor(mColor);
}

QColor SColorWidget::color()
{
    return mColor;
}

void SColorWidget::onColorComponentChanged()
{
    mColor.setRed(mpRedPicker->value());
    mColor.setGreen(mpGreenPicker->value());
    mColor.setBlue(mpBluePicker->value());
    mColor.setAlpha(mpAlphaPicker->value());
    mpColorPreview->setColor(mColor);

    emit colorChanged(mColor);
}

// ----------------------------------------------------------------------------------------
SColorPicker::SColorPicker(eColor color, QWidget *parent, const char *name):QWidget(parent)
  , mpLayout(NULL)
  , mpIndicator(NULL)
  , mpColorValue(NULL)
  , mSettingValue(false)
{
    setObjectName(name);
    mpLayout = new QHBoxLayout();
    mpLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mpLayout);

    mpIndicator = new QLabel(this);
    mpLayout->addWidget(mpIndicator, 0);

    mpColorValue = new SSlider(Qt::Horizontal, this);
    mpColorValue->setMinimum(0);
    mpColorValue->setMaximum(255);
    mpLayout->addWidget(mpColorValue, 1);

    setupColor(color);

    connect(mpColorValue, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

SColorPicker::~SColorPicker()
{
    DELETEPTR(mpColorValue);
    DELETEPTR(mpIndicator);
    DELETEPTR(mpLayout);
}

void SColorPicker::setupColor(eColor c)
{
    switch(c){
    case eColor_R:
        mpIndicator->setText("R");
        mpColorValue->setStyleSheet("QSlider::groove:horizontal{background:qlineargradient(x1:0, x2:1,stop:0 #00FFFF, stop:1 white);}");
        break;
    case eColor_G:
        mpIndicator->setText("G");
        mpColorValue->setStyleSheet("QSlider::groove:horizontal{background:qlineargradient(x1:0, x2:1,stop:0 #FF00FF, stop:1 white);}");
        break;
    case eColor_B:
        mpIndicator->setText("B");
        mpColorValue->setStyleSheet("QSlider::groove:horizontal{background:qlineargradient(x1:0, x2:1,stop:0 #FFFF00, stop:1 white);}");
        break;
    case eColor_A:
        mpIndicator->setText("A");
        mpColorValue->setStyleSheet("QSlider::groove:horizontal{background:qlineargradient(x1:0, x2:1,stop:0 white, stop:1 black);}");
        break;
    }
}

void SColorPicker::setValue(int value)
{
    mSettingValue = true;
    mpColorValue->setValue(value);
    mSettingValue = false;
}

int SColorPicker::value()
{
    return mpColorValue->value();
}

void SColorPicker::onValueChanged(int value)
{
    Q_UNUSED(value);
    if(!mSettingValue){
        emit colorComponentChanged();
    }
}

// -------------------------------------------------------------------------------------------------
SColorPreview::SColorPreview(QWidget *parent, const char *name):QWidget(parent)
  , mpBackColor(NULL)
  , mpForeColor(NULL)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setStyleSheet("background:blue;");
    mpForeColor = new SColorThumbnail(this);
    setMinimumHeight(mpForeColor->height() + 20);
    mpForeColor->setGeometry((width()/2)-(mpForeColor->width()/2), (height()/2)-(mpForeColor->height()/2), mpForeColor->width(), mpForeColor->height());

    // TODO: Add the back/fore color management
}

SColorPreview::~SColorPreview()
{
    DELETEPTR(mpBackColor);
    DELETEPTR(mpForeColor);
}

void SColorPreview::setColor(const QColor &color)
{
    mpForeColor->setColor(color);
}

void SColorPreview::resizeEvent(QResizeEvent* ev)
{
    mpForeColor->setGeometry((width()/2)-(mpForeColor->width()/2), (height()/2)-(mpForeColor->height()/2), mpForeColor->width(), mpForeColor->height());
}

// -------------------------------------------------------------------------------------------------
SColorThumbnail::SColorThumbnail(QWidget *parent, const char *name):QLabel(parent)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setFixedSize(40, 40);
    setColor(Qt::red);
}

SColorThumbnail::~SColorThumbnail()
{

}

void SColorThumbnail::setColor(const QColor &color)
{
    mColor = color;
    QString qsRed;
    QString qsGreen;
    QString qsBlue;
    QString qsAlpha;

    if(color.red() < 16){
        qsRed = QString("0%0").arg(color.red(), 0, 16);
    }else{
        qsRed = QString("%0").arg(color.red(), 0, 16);
    }
    if(color.green() < 16){
        qsGreen = QString("0%0").arg(color.green(), 0, 16);
    }else{
        qsGreen = QString("%0").arg(color.green(), 0, 16);
    }
    if(color.blue() < 16){
        qsBlue = QString("0%0").arg(color.blue(), 0, 16);
    }else{
        qsBlue = QString("%0").arg(color.blue(), 0, 16);
    }
    if(color.alpha() < 16){
        qsAlpha = QString("0%0").arg(color.alpha(), 0, 16);
    }else{
        qsAlpha = QString("%0").arg(color.alpha(), 0, 16);
    }

    setStyleSheet(QString("border:2 solid #222222;background:#%0%1%2").arg(qsRed).arg(qsGreen).arg(qsBlue));
}

QColor SColorThumbnail::color()
{
    return mColor;
}
