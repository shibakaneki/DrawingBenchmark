#include <QDebug>
#include <QApplication>
#include "SSettingsWidget.h"

SSettingsWidget::SSettingsWidget(QWidget *parent, const char *name):QDockWidget(parent)
  , mpContainer(NULL)
  , mpContainerLayout(NULL)
  , mpSmoothnessLabel(NULL)
  , mpSmoothness(NULL)
  , mpInfosLabel(NULL)
  , mpXCoord(NULL)
  , mpYCoord(NULL)
  , mpCoefficientsLabel(NULL)
  , mpTable(NULL)
{
    SETUP_STYLESHEET
    setObjectName(name);
    setMinimumWidth(200);
    mpContainer = new QWidget(this);
    mpContainer->setObjectName("SPaletteContainer");
    setWindowTitle(tr("Debug"));
    setWidget(mpContainer);

    mpContainerLayout = new QVBoxLayout();
    mpContainer->setLayout(mpContainerLayout);

    // Smoothness
    mpSmoothnessLabel = new QLabel(tr("Smoothness"), mpContainer);
    mpSmoothnessLabel->setObjectName("SPaletteTopicTitleLabel");
    mpContainerLayout->addWidget(mpSmoothnessLabel, 0);
    mpSmoothness = new QSlider(Qt::Horizontal, mpContainer);
    mpSmoothness->setStyleSheet("border:none; margin-top:0px;");
    mpSmoothness->setMinimum(1);    // 0%
    mpSmoothness->setMaximum(100);  // 100%
    mpSmoothness->setValue(25);
    mpContainerLayout->addWidget(mpSmoothness, 0);

    // Infos
    mpInfosLabel = new QLabel(tr("Infos"), mpContainer);
    mpInfosLabel->setObjectName("SPaletteTopicTitleLabel");
    mpContainerLayout->addWidget(mpInfosLabel, 0);
    mpXCoord = new QLabel("x: 0", mpContainer);
    mpXCoord->setStyleSheet("border:none;");
    mpContainerLayout->addWidget(mpXCoord, 0);
    mpYCoord = new QLabel("y: 0", mpContainer);
    mpYCoord->setStyleSheet("border:none;");
    mpContainerLayout->addWidget(mpYCoord,0);

    // Coefficients
    mpCoefficientsLabel = new QLabel(tr("Coefficients"), this);
    mpCoefficientsLabel->setObjectName("SPaletteTopicTitleLabel");
    mpContainerLayout->addWidget(mpCoefficientsLabel, 0);
    mpTable = new QTreeWidget(mpContainer);
    QStringList strlHeaders;
    strlHeaders << "p0:x" << "p0:y" << "p1:x" << "p1:y" << "c1:x" << "c1:y" << "c2:x" << "c2:y";
    mpTable->setColumnCount(8);
    mpTable->setHeaderLabels(strlHeaders);
    mpTable->setStyleSheet("padding-right:5px; border:none;");
    mpTable->setAlternatingRowColors(true);
    mpTable->setRootIsDecorated(false);
    mpContainerLayout->addWidget(mpTable, 1);


    connect(mpSmoothness, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
    connect(mpTable, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onPointSelected(QTreeWidgetItem*,QTreeWidgetItem*)));
}

SSettingsWidget::~SSettingsWidget()
{
    DELETEPTR(mpTable);
    DELETEPTR(mpCoefficientsLabel);
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

void SSettingsWidget::onClearCoefficients()
{
    mpTable->clear();
}

void SSettingsWidget::onAddCoefficients(QPointF p0, QPointF p1, QPointF c0, QPointF c1)
{
    mpTableItem = new QTreeWidgetItem(mpTable->invisibleRootItem());
    mpTableItem->setText(0, QString("%0").arg(p0.x()));
    mpTableItem->setText(1, QString("%0").arg(p0.y()));
    mpTableItem->setText(2, QString("%0").arg(p1.x()));
    mpTableItem->setText(3, QString("%0").arg(p1.y()));
    mpTableItem->setText(4, QString("%0").arg(c0.x()));
    mpTableItem->setText(5, QString("%0").arg(c0.y()));
    mpTableItem->setText(6, QString("%0").arg(c1.x()));
    mpTableItem->setText(7, QString("%0").arg(c1.y()));
    mpTable->invisibleRootItem()->addChild(mpTableItem);
}

void SSettingsWidget::onPointSelected(QTreeWidgetItem* crnt, QTreeWidgetItem* prev)
{
    Q_UNUSED(prev);
    if(NULL != crnt){
        QPointF p0, p1, c0, c1;

        p0.setX(crnt->text(0).toInt());
        p0.setY(crnt->text(1).toInt());
        p1.setX(crnt->text(2).toInt());
        p1.setY(crnt->text(3).toInt());
        c0.setX(crnt->text(4).toInt());
        c0.setY(crnt->text(5).toInt());
        c1.setX(crnt->text(6).toInt());
        c1.setY(crnt->text(7).toInt());

        emit pointSelected(p0, p1, c0, c1);
    }
}

void SSettingsWidget::enterEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    QApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void SSettingsWidget::leaveEvent(QEvent *ev)
{
    Q_UNUSED(ev);
    QApplication::restoreOverrideCursor();
}
