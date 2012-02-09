#include "SSettingsWidget.h"

SSettingsWidget::SSettingsWidget(QWidget *parent, const char *name):QDockWidget(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpSmoothnessLabel(NULL)
  , mpSmoothness(NULL)
  , mpInfosLabel(NULL)
  , mpXCoord(NULL)
  , mpYCoord(NULL)
{
    setObjectName(name);
    setMinimumWidth(200);
    mpContainer = new QWidget(this);
    mpContainer->setStyleSheet("border-radius:5px; border:2px solid #B6CFD6; background:white; margin-left: 5px;");

    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    // Smoothness
    mpSmoothnessLabel = new QLabel(tr("Smoothness"), mpContainer);
    mpSmoothnessLabel->setStyleSheet("border:none; font-size:16px; margin-bottom:2px; color:#7CA7B3");
    mpContainerLayout->addWidget(mpSmoothnessLabel, 0);
    mpSmoothness = new QSlider(Qt::Horizontal, mpContainer);
    mpSmoothness->setStyleSheet("border:none; margin-top:0px;");
    mpSmoothness->setMinimum(1);    // 0%
    mpSmoothness->setMaximum(100);  // 100%
    mpSmoothness->setValue(50);
    mpContainerLayout->addWidget(mpSmoothness, 0);

    // Infos
    mpInfosLabel = new QLabel(tr("Infos"), mpContainer);
    mpInfosLabel->setStyleSheet("border:none; font-size:16px; margin-bottom:2px; color:#7CA7B3");
    mpContainerLayout->addWidget(mpInfosLabel, 0);
    mpXCoord = new QLabel("x: 0", mpContainer);
    mpXCoord->setStyleSheet("border:none;");
    mpContainerLayout->addWidget(mpXCoord, 0);
    mpYCoord = new QLabel("y: 0", mpContainer);
    mpYCoord->setStyleSheet("border:none;");
    mpContainerLayout->addWidget(mpYCoord,0);

    mpContainerLayout->addStretch(1);

    connect(mpSmoothness, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

SSettingsWidget::~SSettingsWidget()
{
    DELETEPTR(mpYCoord);
    DELETEPTR(mpXCoord);
    DELETEPTR(mpInfosLabel);
    DELETEPTR(mpSmoothness);
    DELETEPTR(mpSmoothness);
    DELETEPTR(mpContainerLayout);
    DELETEPTR(mpContainer);
}

void SSettingsWidget::onValueChanged(int val)
{
    emit smoothnessChanged(val);
}

void SSettingsWidget::onPosChanged(QPointF pos)
{
    mpXCoord->setText(QString("x:%0").arg(pos.x()));
    mpYCoord->setText(QString("y:%0").arg(pos.y()));
}
